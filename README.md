JoeClient
=========

![Screenshot](https://raw.githubusercontent.com/JoeDavisson/JoeClient/master/screenshots/screenshot.png)

JoeClient is a client for Naken Chat servers. It maintains a user list and saves web links and private messages in case you missed them.

## Build it from source
```$ git clone https://github.com/JoeDavisson/JoeClient.git```

```$ cd JoeClient```

The Makefile supports ```linux``` and ```mingw``` cross-compiler targets.
Typically to build locally you can just type make.

```$ make```

There are some special options for building cross-compiler targets:

```$ PLATFORM=mingw32 make```
or
```$ PLATFORM=mingw64 make```

Under windows you need to set the path to the fltk libraries before building.
Under msys this can be done by setting the following:

```
$ export PATH=$PATH:/drive/to/fltk-1.3.4
$ make
```

## Dependencies

### Libraries

 * FLTK-1.3.3 or higher.
 * libxft-dev (required for font rendering)

