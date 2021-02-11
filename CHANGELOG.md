# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and starting with version 4.1.0 this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [unreleased]

nothing

## [4.1.5] - 2021-02-11

### Added

- Allow disabling the EMCY module by setting NULL for emcy code table.

### Fixed

- Explain how to setup EMCY in quickstart example (even if not used in this application).
- Fix NMT reset when EMCY is not used (no EMCY code table).
- Fix broken RPDO communication setup [cjardin112/master](https://github.com/cjardin112).

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


[unreleased]: https://github.com/embedded-office/canopen-stack/compare/v4.1.5...HEAD
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
