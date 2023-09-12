# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and starting with version 4.1.0 this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [unreleased]

### Change

- Add error checking in COTPdoMapWrite [ezrec](https://github.com/ezrec)
- Fix RPDO dummy mapping [mrk-its](https://github.com/mrk-its)
- Callback for sync and async PDO update [sandrogort](https://github.com/sandrogort)
- Fix Integer16 access for 16bit DSPs

## [4.4.0] - 2022-08-21

### Add

- Add unit tests for all object type functions
- Add CMake presets for simplified usage of CMake in CI/CD
 
### Changed

- Separate all object type functions into separate files
- Adjust repository structure to recommendations in "Professional CMake - A Practical Guide"
- Move website to separate repository

### Attention: This release introduces a breaking change:

Handle basic type implementations with separate type function files and in consequence enlarge the object entry flags from `CO_OBJ_DNPRW` to `CO_OBJ_DNAPRW`. This change required adjustments in existing object dictionary definitions:

```c
old:
  {CO_KEY(0x1001, 0, CO_UNSIGNED8 |CO_OBJ____R_), 0, (CO_DATA)(&Obj1001_00_08)},

new:
  {CO_KEY(0x1001, 0, CO_OBJ_____R_), CO_TUNSIGNED8, (CO_DATA)(&Obj1001_00_08)},
    add an underscore or A ---^         ^
    ensure type starts with CO_T -------'
```

## [4.3.1] - 2022-07-14

### Fixed

- Rollback of 32bit direct storage in 16bit machines (is not compiling reliable)

### Changed

- Add a warning for 16bit microcontroler using direct storage in the object dictionary in docs

## [4.3.0] - 2022-07-13

### Added

- Add a note in documentation to usage and api about the need of sorted object dictionary
- Add support for SDO client segmented transfers [@tonbo777](https://github.com/tonbo777)
- Add support of mapping values with 3 bytes in PDOs
- Introducte callbacks for read/write PDO mapped values larger than 4 bytes

### Changed

- Adjust SDO client API documentation to reflect conditions for expedited and segmented transfers

### Fixed

- Avoid variable declaration if SDO client is disabled [@jernejsk](https://github.com/jernejsk)
- Allow direct data in object dictionaries running on 16bit microcontrollers

## [4.2.0] - 2022-04-01

### Added

- Add callback `CONmtResetRequest()` for handling specific application and/or communication reset features
- Add feature: SYNC producer [@dozack](https://github.com/dozack)
- Add feature: SDO client (limited to expedited transfers) [@dozack](https://github.com/dozack)

### Changed

- Change static website generator to [Material for MkDocs](https://squidfunk.github.io/mkdocs-material/)

### Fixed

- Fix navigation link in sidebar to quickstart example
- Correct restarting of block transfer after aborting a running transfer by a client restart
- Correct abort message when writing to 1014h:00h while active or with value out of range
- Correct abort message when writing to 1003h:00h with value out of range
- Fix data copy loops in NVM simulation read and write functions

## [4.1.8] - 2021-10-04

### Added

- Add the option to exclude LSS and/or parameters [@Domen2242](https://github.com/Domen2242)
- Detailed description of API function `COObjTypeUserSDOAbort()`
- Example article explaining the usage with RTOS and baremetal
- Example article explaining the firmware upload with user domains

### Fixed

- Interface description and examples of API functions on Page "Object Entry"
- SDO segmented download of basic data type
- SDO block upload and download of basic data type
- correct some Domain and String object definitions in documentation

## [4.1.7] - 2021-07-28

### Added

- Introduce `COObjTypeUserSDOAbort()` to define amnufacturer specific SDO Abort codes within type functions
- Add CMake toolchain files for ARM-GCC Cortex-M3, M4 and M7

## [4.1.6] - 2021-07-21

### Added

- Explain the possible PDO transmission options
- Add Example which describes how to setup and use a dynamic object dictionary
- Add type functions for EMCY COB-ID (1014) and SYNC COB-ID (1005)

### Changed

- Clarify the arguments in type functions read() and write()

### Fixed

- PDO Timer Config when entering the OPERATE state multiple times
- Fix the random bogus frame passed to function when no CAN frame is received
- Fix issues in SDO block upload and download protocol

## [4.1.5] - 2021-02-11

### Added

- Allow disabling the EMCY module by setting NULL for emcy code table.

### Fixed

- Explain how to setup EMCY in quickstart example (even if not used in this application).
- Fix NMT reset when EMCY is not used (no EMCY code table).
- Fix broken RPDO communication setup [@cjardin112](https://github.com/cjardin112).

## [4.1.4] - 2020-12-15

### Added

- Allow CAN driver polling for testing and demo purpose (Don't use this in production!).
- Implement a simple NVM driver simulation in RAM.

### Fixed

- Add `CO_TASYNC` in quickstart object entry to trigger PDO transmission on write access.

## [4.1.3] - 2020-10-30

### Added

- Standard changelog file in the project root.

### Fixed

- CO_OBJ_TYPE const qualifier [@cesarvandevelde](https://github.com/cesarvandevelde).
- COBID macros: use 11-bit mask for std IDs [@cesarvandevelde](https://github.com/cesarvandevelde).

## [4.1.2] - 2020-10-23

### Changed

- Improve COBID macro robustness.

### Fixed

- Correct flags of mapped object in source snippet of quickstart documentation.

## [4.1.1] - 2020-10-23

### Added

- Macros for COBID generation.
- Explain PDO mapping in website section: configuration.

### Changed

- Allow change of PDO event timers in operational mode.
- Ensure a SDO response for all SDO requests.

### Fixed

- Fix the double transmission of last frame in a block upload.
- Warning free compilation with clang compiler.
- Correct multiple issues in testsuite.

## [4.1.0] - 2020-10-10

### Added

- Introduce drivers layer for CAN, NVM and Timer.
- Example quickstart project 'clock' as compilable source code.
- Documentation for interfacing the hardware with drivers.
- Driver templates and simulation drivers for testsuite.

### Changed

- Documentation for callback functions.

## [4.0.3] - 2020-10-01

### Changed

- Make timer tick rate configurable.

## [4.0.2] - 2020-09-21

### Added

- Compatibility for C++ project setup.

### Fixed

- Correct errors in source code snippets of quickstart documentation.

## [4.0.1] - 2020-07-14

### Added

- Introduce a new callback when a synchronized PDO is received.

## [4.0.0] - 2020-05-13

### Added

- First Open Source Release.


[unreleased]: https://github.com/embedded-office/canopen-stack/compare/v4.4.0...HEAD
[4.4.0]: https://github.com/embedded-office/canopen-stack/compare/v4.3.1...v4.4.0
[4.3.1]: https://github.com/embedded-office/canopen-stack/compare/v4.3.0...v4.3.1
[4.3.0]: https://github.com/embedded-office/canopen-stack/compare/v4.2.0...v4.3.0
[4.2.0]: https://github.com/embedded-office/canopen-stack/compare/v4.1.8...v4.2.0
[4.1.8]: https://github.com/embedded-office/canopen-stack/compare/v4.1.7...v4.1.8
[4.1.7]: https://github.com/embedded-office/canopen-stack/compare/v4.1.6...v4.1.7
[4.1.6]: https://github.com/embedded-office/canopen-stack/compare/v4.1.5...v4.1.6
[4.1.5]: https://github.com/embedded-office/canopen-stack/compare/v4.1.4...v4.1.5
[4.1.4]: https://github.com/embedded-office/canopen-stack/compare/v4.1.3...v4.1.4
[4.1.3]: https://github.com/embedded-office/canopen-stack/compare/v4.1.2...v4.1.3
[4.1.2]: https://github.com/embedded-office/canopen-stack/compare/v4.1.1...v4.1.2
[4.1.1]: https://github.com/embedded-office/canopen-stack/compare/v4.1.0...v4.1.1
[4.1.0]: https://github.com/embedded-office/canopen-stack/compare/v4.0.3...v4.1.0
[4.0.3]: https://github.com/embedded-office/canopen-stack/compare/v4.0.2...v4.0.3
[4.0.2]: https://github.com/embedded-office/canopen-stack/compare/v4.0.1...v4.0.2
[4.0.1]: https://github.com/embedded-office/canopen-stack/compare/v4.0.0...v4.0.1
[4.0.0]: https://github.com/embedded-office/canopen-stack/releases/tag/v4.0.0
