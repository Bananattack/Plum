         __        _       _     _    __              _ _        __
        / /  _ __ | | __ _(_) __| |  / /_ _ _   _  __| (_) ___   \ \
       / /  | '_ \| |/ _` | |/ _` | / / _` | | | |/ _` | |/ _ \   \ \
       \ \  | |_) | | (_| | | (_| |/ / (_| | |_| | (_| | | (_) |  / /
        \_\ | .__/|_|\__,_|_|\__,_/_/ \__,_|\__,_|\__,_|_|\___/  /_/
            |_|

                              <plaid/audio>
                        standalone audio framework
                               Version 0.1
                      written with <3 by Evan Balster


== PURPOSE ==

plaid/audio is a portable, extensible framework for CPU-processed audio.
It is designed chiefly for use in games and other media-intensive software.

It offers a free (zlib-licensed) alternative to commercial audio middleware.
Unlike the leading system, OpenAL, its behavior is consistent across platforms.


At present it offers classes for streaming and buffering audio files, playing
back microphone input, manipulating streams with a selection of simple effects
including pitch-changing and amplification, as well as mixing and splicing.


plaid/audio's scheduling system synchronizes audio with the program's framerate.
It does this by dividing output into "timeslices" for each call to update().

This allows games to render their audio at their graphical framerate, with any
changes in audio effects, synthesizers or samplers being audible every frame.
Physics, animation or player input can thus have articulate audio feedback.



== INSTALLATION ==

Things are rather spartan at present.  It's quite possible to build a static
library, but the current package does not provide for it.  For now the easiest
way to use <plaid/audio> in a project is simply to drop in its source.



== DOCUMENTATION ==

Forthcoming.
The header files are meant to be relatively readable; they merit a look.



== STRUCTURE ==

This package contains the following:
|
| - /plaid/
|       The plaid/audio library lives here.
|       Put it into your include path and add the sources to your project.
|
| - /imp-portaudio/
|       An implementation layer written with portaudio.
|       Compatible with Windows, OS X, and Linux.
|       You need an implementation to use the engine; add it to your project.
|
| - /portaudio/
|       Header and lib files for portaudio.
|       It was built with Visual C++ 2010 express, with support for ASIO.
|       ASIO is non-free, so you may wish to build your own lib without it.
|
| - /codec_stb/
|       A codec which allows you to stream OGG files.  Add it to your project.
|       You can write your own codecs to support streaming other formats.
|
| - /test.cpp
| - /testproject.*
|       A sample program.  Really more like a song written in C++.  :)
|       A Visual Studio 2010 project is included and ready to build.
|
| - /readme.txt
        A file which, perplexingly, contains its own description.



== LICENSE ==

<plaid/audio> standalone audio framework
Copyright (c) 2012 Evan Balster, Interactopia LLC

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.



== VERSION HISTORY ==

0.1 -- December 04, 2012

First standalone version of plaid/audio.
Includes portaudio implementation and stb_vorbis.c codec.

Includes: Pitch, Amp, Pan, Bandpass, Mixer, Splicer, Oscillator, AudioClip,
	among a few other utility classes.

AudioClip is presently functional but incomplete.
Oscillator is presently functional but just awful at anything besides sines.
