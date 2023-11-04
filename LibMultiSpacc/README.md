# LibMultiSpacc

LibMultiSpacc is a library aiming to provide cross-platform abstractions for a number of less-cross-platform libraries.

The idea is simple: to build an universal abstraction layer on top of other existing libraries, that developers can use in their code instead of the platform-specific calls, to make it possible for an application to get build-time support for a major number of previously incompatible platforms.

The list of supported (or planned) backend libraries follows:

- Current PC platforms, modern mobile platforms and game consoles, the Web, ... via SDL 2.0 (WIP) (and 3.0?[^1])
- Old PC and embedded platforms, ... via SDL 1.2 (WIP)
- MS-DOS (?)
- NDS (WIP)
- GBA (Planned)
- NES (WIP)

SDL is used as the main cross-platform library everywhere possible as stated above, for convenience. Specific platform that require special code are handled separately via other base abstraction layers.

[^1]: I just discovered that SDL 3.0 exists, I'm so tired, why would they make a new major, now I have to support 3 versions... or just drop 2.0 and only support 1.2 + 3.0.
