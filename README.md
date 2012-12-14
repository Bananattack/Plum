Plum
====

About Plum
----------

Plum is a free game engine, used for making cool 2D games with minimal effort through Lua scripting. Its aim to be simple and lightweight, but simultaneously powerful. The performance-intensive heavy lifting and cross-platform work is done in the C++ backend, but the frontend gives a great degree of high-level control via Lua scripts, for a balanced experience that appeases both novices and control freaks.

It supports all the basic needs of a simple 2D game:

* Scripting (via Lua 5.2)
* Hardware-accelerated graphics rendering
* Software canvases for image processing
* Audio playback
* Tilemaps
* Sprites
* Timing
* Keyboard input
* Mouse input
* Joypad input


How to compile
--------------

- **To compile on Windows (Visual Studio 2010):**
  Open up `Plum.sln`, then hit Build Solution, and everything should work.
  The compiled executable is named "Plum.exe".
  - For release builds, it is located in the `test/` folder.
  - For debug builds, it is located in the `Debug/` folder
- **To compile on Linux (GCC):** TODO.

Licensing
---------

See `license.md` for more details.
