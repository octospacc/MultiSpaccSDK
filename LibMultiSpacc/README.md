# LibMultiSpacc

LibMultiSpacc is a library aiming to provide cross-platform abstractions for a number of less-cross-platform libraries.

The idea is simple: to build an universal abstraction layer on top of other existing libraries, that developers can use in their code instead of the platform-specific calls, to make it possible for an application to get build-time support for a major number of previously incompatible platforms.

The list of supported (or planned) platforms and backend libraries follows:

- Current PC platforms, modern mobile platforms and game consoles, the Web, ... via SDL 2.0 (WIP) (and 3.0?[^1])
	- Including: desktop Linux, Windows, Android, Switch, ...
- Old PC and embedded platforms, ... via SDL 1.2 (WIP)
	- Including: Windows 9x, 3DS, ...
- MS-DOS, Windows 3.1 (Maybe?)
- NDS via libnds (WIP)
- GBA (Planned)
- PS1/PS2 (Planned)
- NES, via neslib (WIP)

SDL is used as the main cross-platform library everywhere possible as stated above, for convenience. Specific platform that require special code are handled separately via other base abstraction layers.

[^1]: SDL 3.0 is the current experimental version of SDL, currently only 2.0 is considered stable. SDL 3.0 will be supported by LibMultiSpacc only if it grants any actual benefits (like better performance with equal code, or support for exclusive platforms) when it eventually becomes stable.

## Experimental notice

This project is still in an experimental phase. Tagged releases will eventually be made available with time, but currently you should avoid using the library for new projects by keeping sync with the development branch, as the API is constantly changing, and so things will break for you.

## Licensing

* The LibMultiSpacc core code is licensed under "GNU Lesser General Public License version 3 with Static Linking Exception", meaning you can use the library for any program, with any license, for any purpose, but if you modify part of the library core then you have to make that part (not necessarily your entire program) available in source code form under the original license. See [LICENSE.txt](./LICENSE.txt) for legal details.
* The included examples for the library, both in code and media assets, will be licensed under an extremely permissive license that allows full remixing without any additional licensing/distribution requirements/obligations, but I currently don't know which would be best.
* Any included third-party support library, or third-party borrowed media assets, fall under their own specified licenses.
