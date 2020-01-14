The Butterfly Effect Installation Instructions
==============================================

This file explains step-by-step how to build TBE from the source archive,
for GNU/Linux, Windows and macOS.

If you downloaded an installer and executed it, being able to read
this file already suggests you're done - the installer did its work.

Building on GNU/Linux
---------------------

### Preparation

Make sure that you have all tools & libraries installed that you need.

These include:

* gcc, glibc-devel
* binutils
* make
* cmake
* patch

Depending on whether you have these:

* libqt5 (which must be Qt version 5.2 or later)
* libqt5-devel
* libqt5xml5
* libqt5svg5
* libqt5svg5-dev
* qttools5-dev
* qttools5-dev-tools

If you do not, you'll need to build the static version, which in turn
requires building Qt first; you'll now also need:

* a lot of X11 development libraries
* wget

However, doing a static build is beyond the scope of this document,
please refer to the TBE wiki:
https://github.com/the-butterfly-effect/tbe/wiki/HowToMakeARelease

### Building

As a _normal_ user, build the main project.
If you have CMake installed, this should be as easy as just run make in this
directory:

    make

That make will in turn call cmake and build everything for you.
It will also immediately install the game in the local directory and install
a link to the binary `tbe` in the project root.

### Running

You can now start the game by typing: `./tbe`

Enjoy!


### Release build

If you want to create a release build instead, see `RELEASING.md`

Building for Windows
--------------------

This section describes how to build TBE for Windows 10, 32-bit from source.
It should be similar to build for Windows 8 or 7. We don't build on older platforms anymore.

For now, this page is a work-in-progress.

### Installing various packages

Note: Below are the versions of packages as used by Klaas to build release `0.9.3`. If you are reading this page, probably a few packages have already released newer versions. Apart from Qt, which must remain at version 5.2 or higher, all other packages are relatively free in version numbers, so at your preference please pick a later version.

Install the following packages:
 * from [7-zip.org](7-zip.org), download and install 7-zip: `7z1507-x86.exe`
 * from the mingw-w64 project on sf.net, download, extract and install `i686-4.8.2-release-posix-dwarf-rt_v3-rev3.7z`
 * from [qt.io](qt.io): download and install the latest qt5 for windows package. For us, this was `qt-unified-windows-x86-2.0.2-2-online`
 * also from [qt.io](qt.io): download and install `qt-creator-opensource-windows-x86-3.5.0.exe`
 * from [git-scm.com](git-scm.com): download and install `git-2.5.3-64-bit.exe`
 * from [cmake.org](cmake.org): download and install `cmake-3.3.2-win32-x86.exe`. You do not need to add cmake to the system PATH or create an icon.
 * from [nsis.sf.net](nsis.sf.net): download and install `nsis-3.0b2-setup.exe`.

### Getting the source code
To download the source code, do the following:
 * Start the git GUI.
 * Select _Clone Existing Repository_
 * Enter `https://github.com/the-butterfly-effect/tbe.git` as _Source Location_
 * Enter `C:\TBE` as _Target Directory_
 * Click the _Clone_ button
 * It will take a while, but the application will complete the cloning and the real Git GUI appears. If you take the Windows Explorer, you'll also see that the `C:\TBE` directory is now filled with files.

**TODO:** Set tree to a specific tag or branch

### Configuring

To configure and build the source code, do the following:
 * Start Qt Creator.
 * Click _Open Project_.
 * Browse to the source code (_C:\TBE_). In my case, Qt Creator started moaning about "kits" - but incorrectly about a CMake kit. But it had found CMake, it just hadn't found Qt, compiler and gdb.
   * click on the _Qt versions_ tab
     * click on the _Add..._ button and locate your `qmake` binary (_C:\Qt\5.5\mingw492_32\bin_)
     * click _Apply_
   * click on the _Compilers_ tab
     * click on the _Add_ button
     * click on _MinGW_
     * there are new fields below the name/type listbox, click the _Browse..._ button of the _Compiler path:_
       * select _C:\mingw32\bin\g++.exe_, this will now also enable a combo box at the bottom for _ABI_.
     * select _Apply_
   * click on the _Debuggers_ tab
     * click on the _Add_ button
     * locate the gdb binary in _C:\mingw32\bin\gdb.exe_
     * click _Apply_





Building on macOS
-----------------

Note: This section is quite chaotic and needs updating!

We used the following configuration to build TBE for macOS:

* macOS Sierra 10.12.1 64-bit
* Compiler: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ --version
  Apple LLVM version 8.0.0 (clang-800.0.42.1)
  Target: `x86_64-apple-darwin16.1.0`
  Thread model: posix
* Qt 5.7.0
* cmake                          3.6.2           HomeBrew
* expat                          @2.2.0          textproc/expat
* gettext                        @0.19.8.1       devel/gettext
* libiconv                       @1.14           textproc/libiconv
* ncurses                        @6.0            devel/ncurses
* git                            2.10.2          HomeBrew

Currently[MacPorts](http://www.macports.org) was used to install the needed libraries, will probably move to [Homebrew](http://brew.sh).

### Configuration
Install the above packages.

### Building for local run

#### Compilation
Just run:

    make distclean all

#### Running locally
Just type:

    ./`pwd`/build/installprefix/tbe.app/Contents/MacOS/tbe

Or double click `pwd`/build/installprefix/tbe.app

You can copy this to e.g. the Applications folder.

### Building for packaging
It will (once done) be as simple as:

    cd build
    make package

This doesn't work at the moment, work in progress.

### TODO:
See [Improve OS X support #226](https://github.com/the-butterfly-effect/tbe/issues/226)
