# Dynamic Object Dictionary - Part I

Projects using CANopen may need to realize an adaptive CANopen Object Dictionary. For example, the CANopen device acts as a standard IO-Device or as a proprietary device - depending on external system settings. In this example, we will discover the needed steps to get a fully dynamic CANopen Object Dictionary for our CANopen Stack.

## Functional Specification

This example is the first of two writings to achieve a flexible and comfortable way of managing CANopen device software with dynamically generated object dictionaries during runtime.

The first part describes tool functions for managing the object directory, while the [second part](/examples/use-dynamic-object-dictionary) illustrates the usage of the introduced tool functions.


### Resource Estimation

The CANopen stack is optimized for minimal resource requirements. The default examples and descriptions assume that you want to minimize the usage of RAM. For this reason, the manuals and templates declare the object dictionary storage as a `const` array which is placed into FLASH memory.

For an estimation of how big the difference is, we have a quick look into the manual. The manual explains that each object dictionary entry needs 3 x 32bit values. Therefore, at an object dictionary with 256 object entries, we talk about: 256 * 12 Byte = 3 kByte

<figure markdown>
  ![CANopen Dictionary Memory Estimation](/assets/images/illustrations/canopen-dictionary-memory.svg "CANopen Dictionary Memory Estimation"){ width=480px }
</figure>

In case of a static object dictionary, the CANopen stack stores the entire dictionary (including pointers to the changing data) in non-volatile memory. On the dynamic side, the CANopen stack stores the object dictionary in RAM, along with the changing data. The use of pointers to the changing data is optional.

Note, the term "*dynamic*" means that we can create and change an object entry within the object dictionary during runtime up to a defined amount of object entries.
{.note}

The allocation of the required memory is still "*static*" - we don't need (and want) to use dynamic memory allocation in embedded systems with all the well-known drawbacks.

Finally: We can improve the flexibility of our CANopen application software when working with a dynamic object dictionary.

## Module: app_dict

The object dictionary is an array of object entries. We must store the object entries ascending by the index/sub-index information. This requirement ensures that the CANopen stack can perform fast and efficient access to a specific entry.


### Object Definition

Let's start with introducing a new module, called `app_dict` (application dictionary). This module is responsible for the object directory management. First, we define a structure for holding the variable data of the dynamic object directory:

```c
typedef struct {
  CO_OBJ    *Root;
  uint32_t   Length;
  uint32_t   Used;
} OD_DYN;
```

The member `Root` will hold the start address of the object dictionary. The member `Length` stores the maximal number of object entries in the object dictionary, and the member `Used` will hold the current number of used object entries.


### Local Helper

Some small local functions will help us to get the following implementations of the object dictionary management functions as readable as possible.

The service `ObjSet()` allows us to set the values of a single object entry to the given values.

```c
static void ObjSet(CO_OBJ *obj, uint32_t key, const CO_OBJ_TYPE *type, uintptr_t data)
{
  obj->Key  = key;
  obj->Type = type;
  obj->Data = data;
}
```

The service `ObjCpy()` will copy all values from the second given object entry to the first given object entry.

```c
static void ObjCpy(CO_OBJ *a, CO_OBJ *b)
{
  a->Key  = b->Key;
  a->Type = b->Type;
  a->Data = b->Data;
}
```

The service `ObjSwap()` will exchange the content of the given object entries.

```c
static void ObjSwap(CO_OBJ *a, CO_OBJ *b)
{
  CO_OBJ x;

  ObjCpy(&x,  a);
  ObjCpy( a,  b);
  ObjCpy( b, &x);
}
```

The service `ObjCmp()` will compare the index and subindex information and return the result of the comparison similar to `strcmp()`

```c
static int16_t ObjCmp(CO_OBJ *a, CO_OBJ *b)
{
  int16_t result = 1;

  if (CO_GET_DEV(a->Key) == CO_GET_DEV(b->Key)) {
    result = 0;

  } else if (CO_GET_DEV(a->Key) < CO_GET_DEV(b->Key)) {
    result = -1;
  }

  return (result);
}
```


### Initialization Function

When starting the CANopen node, we have to specify the object dictionary as an argument for this node.

The next service function will provide us a clean and empty object dictionary.

```c
void ODInit(OD_DYN *self, CO_OBJ *root, uint32_t length)
{
  uint32_t  idx;
  CO_OBJ    end = CO_OBJ_DIR_ENDMARK;
  CO_OBJ   *od;

  idx = 0;
  od  = root;
  while (idx < length) {
    ObjCpy(od, &end);
    od++;
    idx++;
  }

  self->Root   = root;
  self->Length = length - 1;
  self->Used   = 0;
}
```

Note the marked line for setting the maximal number of object entries. The CANopen stack requires a `CO_OBJ_DIR_ENDMARK` at the end of the used object dictionary area. For this reason, we reduce the maximal number of (user) object entries by 1.


### Add & Update Function

The function for adding a new object entry needs three arguments besides the object handle reference `self`:

- First argument is the `key` which includes the information of index, subindex, type and some additional flags to a single 32bit value. We should create this value with the macro CO_KEY(..).

- The second argument, `type`  is a reference to a structure of type-functions. For primitive object types, this argument is 0. The CANopen reference manual explains the system data types. Some selected system data types are described in later examples, too.

- The last argument `data` holds the data of this object entry. The interpretation of this data depends on the flags in the argument `key`. We have a look at this relationship later in the examples.

```c
void ODAddUpdate(OD_DYN *self, uint32_t key, const CO_OBJ_TYPE *type, uintptr_t data)
{
  CO_OBJ  temp;
  CO_OBJ *od;

  if ((key == 0) ||
    (self->Used == self->Length)) {
    return;
  }

  od = self->Root;
  ObjSet(&temp, key, type, data);

  /* find position in dictionary */
  while ((od->Key != 0) && (ObjCmp(od, &temp) < 0)) {
    od++;
  }

  if (ObjCmp(od, &temp) == 0) {        /* Change existing entry */
    ObjSet(od, key, type, data);

  } else if (od->Key == 0) {           /* Append at end of dictionary */
    ObjSet(od, key, type, data);
    self->Used++;

  } else {                             /* Insert in middle of dictionary */
    do {
      ObjSwap(od, &temp);
      od++;
    } while (od->Key != 0);
    ObjCpy(od, &temp);
    self->Used++;
  }
}
```

The generation of the object dictionary executes during the initialization phase of the CANopen node. Therefore the performance is not the most critical part. Nevertheless, we want to have an algorithm, which is linear ( O(n) ) when raising the number of object entries.

In principle, we want to walk through the (sorted) list one time and insert the new object entry in the right position. All used object entries behind the insert position must be shifted by one place to the end.


## Conclusion

That's it. Now we have a set of functions for managing the object dictionary. This set includes services for:

* Object Handling (Set, Copy, Swap Compare)
* Object Dictionary Handling (Init, Add & Update)

We are now ready to go for using these functions in the next example: [Dynamic Object Dictionary - Part II](/examples/use-dynamic-object-dictionary).
