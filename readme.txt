Plum
====

About Plum
----------

Plum is a free game engine, that will one day be used for making cool 2D
games with minimal effort and maximum awesome through Lua scripting, a mixture
of software and hardware rendering, tilemaps, spritesheets, and other fun things.

Under the hood, Plum uses Audiere for sound mixing, Corona for loading images,
SDL for event handling, OpenGL for rendering, zziplib for packfiles,
and a few other things.

Plum takes some inspiration from ika (http://ika.sf.net)
and Verge (http://verge-rpg.com), because these were game-making
communities I've been a part of over the years. They've shaped what I
expect out of an engine, and helped me as a developer as I was growing up.

It is very much a work-in-progress still, as many core features are not yet 
implemented. But it is slowly making strides. 

Plum is not an acronym, as you can notice by the spelling, but for hilarity, I
have come up with a few eccentric ones that would fit:
    Perfectly Ludicrous Universe Maker
    Poetically Lavish Universe Maker
    Platonically Loving Unicorn Machine

How to compile from git
-----------------------

- To compile from git on Windows:

I assume you already know how to use git. If not learn the basics, then come back.
You need to install Visual Studio 2008.
If you're using Express edition, you will need other dependencies.
For certain, you will require the Windows Platform SDK.

If any finds out there are other things they need installed, please
contact me.

Anyways, just open up "plum.sln" and right-click Build Solution.
If it worked, you should get a successful output in the build log.
The compiled executable is named "Plum.exe"
Release builds are compiled to the "test/" folder.
Debug builds are compiled to a folder named "Debug/".
If you're pushing changes to me, please use release mode.

- To compile from git on Linux:

I assume you already know how to use git. If not learn the basics, then come back.
You need the build-essentials, like gcc and make.
You will also need to hae SDL installed.
Potentially other things, I'm not very good at Linux.

Anyways, just run "make" in the base directory of the git repo.
With that, you will get a compiled executable named "Plum",
which should appear in the "test/" folder.

If you're good at Linux, and want to help make my awesome game
engine more portable, please contact me!

Licensing
---------

Plum is released under a 3-clause BSD license. For more information see "license.txt"

What's Planned
--------------

Plum is currently under development. To take a glance at what development is planned,
please look over "todo.txt"

About the Author
----------------

Andrew G. Crowell is an indie game developer.

He was once a student, and completed the final year of his Computer Science
degree program in April 2010.

Read more about him and his lame attempts at game dev here: http://www.bananattack.com/