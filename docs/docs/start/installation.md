---
layout: article
title: Installation
sidebar:
  nav: docs
aside:
  toc: true
---

## Getting the Source

All you need to do is copying all files to your local drive. There are several ways to do so:

### Clone Repository

Clone [canopen-stack](https://github.com/embedded-office/canopen-stack) from github:

```
git clone https://github.com/embedded-office/canopen-stack.git
```

### Download ZIP File

Download and unzip the file from repository [canopen-stack](https://github.com/embedded-office/canopen-stack):

- Click on `Clone or download`
- Select `Download ZIP`
- Unzip the archive to your local drive

### Fork Repository

Fork the github repository [canopen-stack](https://github.com/embedded-office/canopen-stack)

## Cross-compile for Target

Add the source files and include paths to a library project (see [Structure](/docs/start/structure)). Create a library with your cross-compiler and share this library together with the include files with your embedded software project.
