---
layout: page
---

![Welcome to The Butterfly Effect](images/web-title_page.png)

 * [Features](#Features) * [Downloads](#Downloads) * [Open Source](#OpenSource) * 


The Butterfly Effect is a free and open source game that uses realistic physics simulations to combine lots of simple mechanical elements to achieve a simple goal in the most complex way possible.

This is a fully playable game of alpha quality, well on its way to a shiny 1.0 release. For anyone who has played earlier versions of the game, you'll notice a shiny new user interface - but the same levels and objects.

### Features <a id="Features"></a>

_The Butterfly Effect_ has been over five years in the making. 
That means that even though the game has been designed by hobbyists, the game is rich in features:
 * Many different objects: cola bottles, balloons, butterflies, domino stones, springs, dynamite
 * Many different types of puzzles
 * Works on Linux and Windows
 * Playable in English, Russian, Dutch, Spanish and Malay, but not all languages are 100% yet
 * Using vector graphics - all images always rendered at the highest quality
 * There are 45 levels. All levels are playable, but a few levels may need a bit more polish.

### Download & Install <a id="Downloads"></a>

We are working on getting packages for major Linux-distributions as well as a Windows-installer.

Operating System | MD5SUM | Download
---------------- | ------ | --------
Debian / Ubuntu  |  TBD   |  TBD.deb
Fedora / CentOS  |  TBD   |  TBD.rpm
Opensuse         |  TBD   |  TBD.rpm
Mageia Linux     |   n/a  | part of distribution
Other Linux      |  TBD   |  TBD.sh (self extracting archive)
Windows          |  TBD   |  TBD.exe

### Installation instructions

#### RPM-based distributions

Every RPM-based distribution allows to use the `rpm` command to install the RPM
package like this (make sure to supply the correct file name):
```
    rpm -ivh thebutterflyeffect.rpm
```

If, however, RPM detects that there are missing dependencies (i.e. the game
needs other libraries or packages that are not yet installed), the resolution
depends on the version of Linux you are using:
 * Fedora / CentOS: use `yum` to install, the yum tool itself will figure out
   missing dependencies and install them.
 * Opensuse: use `zypper` to install, the zypper tool itself will figure out
   missing dependencies and will download & install them:

Of course, you can also use graphical package managers.


#### DEB-based distributions

TODO

#### Self-extracting archive

TODO

#### Windows

TODO


### Open Source <a id="OpenSource"></a>

This game is released under the **GPLv2** license.
That _The Butterfly Effect_ is open source, means that you can build the game yourself, look how we did it and modify the code or the levels.
We'd love for you to submit your changes back to us - we love feedback and will try to incorporate your changes into the next version of the game.

That's what open source is about: as a community we try to make the best game possible!

The Butterfly Effect also wouldn't have been possible without other open source
projects, including:
 * **Qt** library for the graphical user interface
 * **Box2D** library for physics simulations
 * **CMake/CPack** tools for configuring and building applications
 * **NSIS** the Nullsoft Installer is a program to build Windows installer tools
 * **Inkscape** is a program to draw vector graphics
 * **Openclipart.org** is a website to share clipart graphics

#### Help out with translations

TODO, we use Transifex

#### Help out with new or improved levels

#### Add more code

#### Found any bugs?


