Plum
====

***by Andrew G. Crowell (Overkill / Bananattack)***

About
-----

Plum is a free game engine for making 2D games in Lua.
Plum's aim is to be simple and light and quick-to-learn, but simultaneously powerful and performant.
You get easy, usable components out of the box, but there is a great degree of finer control when you need it.
The result is a balanced game-making experience that should appease both new people and control freaks.

It supports all the basic needs of a simple 2D game:

* Scripting with Lua 5.2
* Hardware-accelerated graphics rendering
* Software canvases for image processing
* Audio playback
* Tilemaps
* Sprites
* Timing
* Keyboard input
* Mouse input
* Joystick input
* Files

Compiling
---------

- **To compile on Windows (Visual Studio 2013):**
  Open up `plum.sln`, then hit Build Solution, and everything should work.
  The compiled executable is named "plum.exe".
  - For release builds, it is located in the `test/` folder.
  - For debug builds, it is located in the `Debug/` folder
- **To compile on Mac OS X:** TODO.
- **To compile on Linux (Ubuntu 12.04):**
  - First, acquire the necessary packages:

    ```
  sudo apt-get install build-essential mesa-common-dev libglu1-mesa-dev \
        libx11-dev libxxf86vm-dev libxrandr-dev libxi-dev libasound2-dev
    ```
  
  - Next, open a terminal and type `make` at the root of this source tree.
  - If it succeeds, there should be an excutable named "plum" located in the `test/` folder.

License
-------

**Plum** is released under the [BSD 3-Clause License](http://opensource.org/licenses/BSD-3-Clause):.
It frees you to contribute, to create derivative works, to use it in commercial projects, among other things
without too much thought -- so long as you follow the conditions of the license, which are pretty friendly.

I don't really care that much about what you do with it, I'm giving you something that I worked on, and maybe it's useful.
Please read the `license.md` file included in this repository for full details though.
