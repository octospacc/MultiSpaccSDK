# LibMultiSpacc

LibMultiSpacc is a library aiming to provide cross-platform abstractions for a number of less-cross-platform libraries.

The idea is simple: to build an universal abstraction layer on top of other existing libraries, that developers can use in their code instead of the platform-specific calls, to make it possible for an application to get build-time support for a major number of previously incompatible platforms.

The list of supported (or planned) backend libraries follows:

- SDL 1.2 (WIP)
- SDL 2.0/3.0 (WIP)[^1]
- NDS (Planned)
- GBA (Planned)
- NES (Planned)

Example programs (and makefiles) will come soon.

[^1]: I just discovered that SDL 3.0 exists, I'm so tired, why would they make a new major, now I have to support 3 versions... or just drop 2.0 and only support 1.2 + 3.0.
