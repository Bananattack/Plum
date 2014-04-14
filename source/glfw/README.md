# GLFW

## Introduction

GLFW is a free, Open Source, portable library for OpenGL and OpenGL ES
application development.  It provides a simple, platform-independent API for
creating windows and contexts, reading input, handling events, etc.

Version 3.0.4 is *not yet described*.  As this is a patch release, there are no
API changes.

If you are new to GLFW, you may find the
[introductory tutorial](http://www.glfw.org/docs/latest/quick.html) for GLFW
3 useful.  If you have used GLFW 2 in the past, there is a
[transition guide](http://www.glfw.org/docs/latest/moving.html) for moving to
the GLFW 3 API.


## Compiling GLFW

See the [Compiling GLFW](http://www.glfw.org/docs/latest/compile.html) guide in
the GLFW documentation.


## Using GLFW

See the
[Building programs that use GLFW](http://www.glfw.org/docs/latest/build.html)
guide in the GLFW documentation.


## Changelog

 - Renamed configuration header to `glfw_config.h` to avoid conflicts
 - Bugfix: The `glfw3.pc` file did not respect the `LIB_SUFFIX` CMake option
 - [Win32] Bugfix: Removed joystick axis value negation left over from GLFW 2
 - [Win32] Bugfix: Restoring windows using the Win+D hot key did not trigger the
                   focus callback
 - [Win32] Bugfix: The disabled cursor mode clip rectangle was updated for
                   unfocused windows
 - [Cocoa] Added dependency on CoreVideo framework for refresh rate retrieval
 - [Cocoa] Enabled Lion full screen for resizable windowed mode windows
 - [Cocoa] Bugfix: The `GLFW_KEY_GRAVE_ACCENT` key was reported as
                   `GLFW_KEY_WORLD_1` and vice versa
 - [Cocoa] Bugfix: The `GLFW_KEY_F13` key was reported as
                   `GLFW_KEY_PRINT_SCREEN`
 - [Cocoa] Bugfix: Implicit conversion from `NSUInteger` to int caused warnings
                   with Xcode 5
 - [Cocoa] Bugfix: Use of undeclared selectors with `@selector` caused warnings
                   with Xcode 5
 - [Cocoa] Bugfix: The cursor remained visible if moved onto client area after
                   having been set to hidden outside it
 - [Cocoa] Bugfix: The refresh rate was zero for all modes of certain monitors
 - [Cocoa] Bugfix: The `install_name` field of the dynamic library was not set
 - [Cocoa] Bugfix: Full screen windows were never reported as having focus
 - [Cocoa] Bugfix: A superfluous I/O flag test prevented video modes from being
                   listed for Thunderbolt monitor
 - [Cocoa] Bugfix: Retrieving the name of some external displays caused segfault
 - [Cocoa] Bugfix: The 10.9 SDK defines `GLintptrARB` and `GLsizeiptrARB`
                   differently from the Khronos `glext.h`
 - [X11] Added setting of the `WM_CLASS` property to the initial window title
 - [X11] Bugfix: Removed joystick axis value negation left over from GLFW 2
 - [X11] Bugfix: The position of hidden windows was ignored by Metacity
                 and Compiz


## Contact

The official website for GLFW is [glfw.org](http://www.glfw.org/).  There you
can find the latest version of GLFW, as well as news, documentation and other
information about the project.

If you have questions related to the use of GLFW, we have a
[support forum](https://sourceforge.net/p/glfw/discussion/247562/), and the IRC
channel `#glfw` on [Freenode](http://freenode.net/).

If you have a bug to report, a patch to submit or a feature you'd like to
request, please file it in the
[issue tracker](https://github.com/glfw/glfw/issues) on GitHub.

Finally, if you're interested in helping out with the development of GLFW or
porting it to your favorite platform, we have an occasionally active
[developer's mailing list](https://lists.stacken.kth.se/mailman/listinfo/glfw-dev),
or you could join us on `#glfw`.


## Acknowledgements

GLFW exists because people around the world donated their time and lent their
skills.

 - Bobyshev Alexander
 - artblanc
 - arturo
 - Matt Arsenault
 - Keith Bauer
 - John Bartholomew
 - Niklas Behrens
 - Niklas Bergström
 - Doug Binks
 - blanco
 - Lambert Clara
 - Andrew Corrigan
 - Noel Cower
 - Jarrod Davis
 - Olivier Delannoy
 - Paul R. Deppe
 - Jonathan Dummer
 - Ralph Eastwood
 - Gerald Franz
 - GeO4d
 - Marcus Geelnard
 - Stefan Gustavson
 - Sylvain Hellegouarch
 - heromyth
 - Paul Holden
 - Toni Jovanoski
 - Osman Keskin
 - Cameron King
 - Peter Knut
 - Robin Leffmann
 - Glenn Lewis
 - Shane Liesegang
 - Дмитри Малышев
 - Martins Mozeiko
 - Tristam MacDonald
 - Hans Mackowiak
 - Kyle McDonald
 - David Medlock
 - Jonathan Mercier
 - Marcel Metz
 - Kenneth Miller
 - Bruce Mitchener
 - Jack Moffitt
 - Jeff Molofee
 - Jon Morton
 - Pierre Moulon
 - Julian Møller
 - Ozzy
 - Andri Pálsson
 - Peoro
 - Braden Pellett
 - Arturo J. Pérez
 - Pieroman
 - Jorge Rodriguez
 - Ed Ropple
 - Riku Salminen
 - Sebastian Schuberth
 - Matt Sealey
 - SephiRok
 - Steve Sexton
 - Systemcluster
 - Dmitri Shuralyov
 - Daniel Skorupski
 - Bradley Smith
 - Julian Squires
 - Johannes Stein
 - Justin Stoecker
 - Nathan Sweet
 - TTK-Bandit
 - Sergey Tikhomirov
 - Samuli Tuomola
 - urraka
 - Jari Vetoniemi
 - Simon Voordouw
 - Torsten Walluhn
 - Jay Weisskopf
 - Frank Wille
 - yuriks
 - Santi Zupancic
 - Lasse Öörni
 - All the unmentioned and anonymous contributors in the GLFW community, for bug
   reports, patches, feedback, testing and encouragement

