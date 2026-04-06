JoeClient
=========

![Screenshot](https://raw.githubusercontent.com/JoeDavisson/JoeClient/master/screenshots/screenshot.png)

JoeClient is a client for Naken Chat servers. It maintains a user list and saves web links and private messages in case you missed them.

## Build it from source

*Warning: work in progress.*

The official FLTK source tree must be in this directory.
run ```make fltklib```, then ```make```.

libxft-dev should be installed before compiling FLTK on linux
(otherwise you'll have ugly, non-resizable fonts).

The Makefile supports ```linux``` and ```mingw``` cross-compiler targets, and is configured for static linking. Please run this the first time:

```make fltklib```

followed by:

```make```

Change the ```PLATFORM``` variable in the Makefile if you want to cross-compile for Windows:

```PLATFORM=mingw32```

or

```PLATFORM=mingw64```

### Libraries

 * FLTK-1.4.4
 * libxft-dev (required for font rendering on Linux)

