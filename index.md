---
layout: page
---

The Butterfly Effect is a free and open source game that uses realistic physics simulations to combine lots of simple mechanical elements to achieve a simple goal in the most complex way possible.

This is a fully playable game of alpha quality, well on its way to a shiny 1.0 release. For anyone who has played earlier versions of the game, you'll notice a sparkling new user interface - but the same levels and objects.

## Features

_The Butterfly Effect_ has been over five years in the making. That means that even though the game has been designed by hobbyists, the game is rich in features:

 * Many different objects: cola bottles, balloons, butterflies, domino stones, springs, dynamite
 * Many different types of puzzles
 * Works on Linux and Windows
 * Playable in English, Russian, Dutch, Swedish, Spanish, German, Malay, and several other languages. Note that not all languages are 100% [yet](#HelpTranslating)
 * Using vector graphics - all images always rendered at the highest quality
 * There are 45 levels. All levels are playable, but a few levels may need a bit more polish.

## Screenshots

<div class="row">
    <div class="col-md-offset-1 col-md-8">
        <div class="row">
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-angry-birds2.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-angry-birds2.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-draft-dialbforboom.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-draft-dialbforboom.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-draft-find-the-message.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-draft-find-the-message.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
        </div><!-- /.row -->
        <div class="row">
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-draft-save-the-butterfly.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-draft-save-the-butterfly.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-games-extreme-tux-racer.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-games-extreme-tux-racer.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "images/lvl-picnic-picnic-1.png" | prepend: site.baseurl }}"><img src="{{ "images/lvl-picnic-picnic-1.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
        </div><!-- /.row -->
    </div><!-- /.col-md-off-set-1 col-md-8 -->
</div>

(the screenshots are clickable)

## Download

We are working on getting packages for major Linux-distributions as well as a Windows-installer and maybe even a MacOSX package.

| Operating System       | MD5SUM | Download | Notes |
|:-----------------------|:------:|:--------:|:------|
| Debian / Ubuntu / Mint |  TBD   |  [thebutterflyeffect.deb](https://github.com/kaa-ching/tbe/releases/download/untagged-1c9762fc41c82662ef2a/thebutterflyeffect-0.9.2.1-Linux.deb) | Ubuntu 14.04 or newer |
| Fedora / CentOS        |  TBD   |  [thebutterflyeffect.rpm](https://github.com/kaa-ching/tbe/releases/download/untagged-1c9762fc41c82662ef2a/thebutterflyeffect-0.9.2.1-Linux.rpm) | |
| Opensuse               |  TBD   |  [thebutterflyeffect.rpm](https://github.com/kaa-ching/tbe/releases/download/untagged-1c9762fc41c82662ef2a/thebutterflyeffect-0.9.2.1-Linux.rpm) | Validated to work on Opensuse 13.2 |
| Mageia Linux           |  n/a   |  part of Mageia | |
| Other Linux            |  TBD   |  [thebutterflyeffect.sh](https://github.com/kaa-ching/tbe/releases/download/untagged-1c9762fc41c82662ef2a/thebutterflyeffect-0.9.2.1-Linux.sh) (self extracting archive) | make sure to install Qt4 4.7+ |
| Windows                |  TBD   |  TBD.exe | Windows 7 or newer |

_if you are a packager and want to add TBE to a distribution or build for another platform, do not hesitate to [contact the developers](#BugsFeedback)_

If you need them, you can refer to our [installation instructions](install).

## Open Source

This game is released under the **GPLv2** license. That _The Butterfly Effect_ is open source, means that you can build the game yourself, look how we did it and modify the code or the levels. We'd love for you to submit your changes back to us - we love [feedback](#BugsFeedback) and will try to incorporate your changes into the next version of the game.

That's what open source is about: as a community we try to make the best game possible!

The Butterfly Effect also wouldn't have been possible without other open source projects, including:

 * [**Qt**](https://www.qt.io/) library for the graphical user interface
 * [**Box2D**](http://box2d.org/) library for physics simulations
 * [**CMake/CPack**](https://cmake.org) tools for configuring and building applications
 * [**NSIS**](http://nsis.sourceforge.net) the Nullsoft Installer is a program to build Windows installer tools
 * [**Inkscape**](https://inkscape.org/en/) is a program to draw vector graphics
 * [**Openclipart.org**](https://openclipart.org) is a website to share clipart graphics
 * [**git**](https://git-scm.com/) is a tool for source code management. It is the backbone of [GitHub](https://github.com)
 
To be honest, we also use a few commercial offerings, including:

 * [**GitHub**](https://github.com) a website that hosts source code and websites
 * [**Coverity**](https://scan.coverity.com/) a tool to find software bugs through _static analysis_
 * [**Pareon Verify**](https://pareonverify.com) a tool to find software bugs though _dynamic analysis_
 * [**Transifex**](https://www.transifex.com/) a website to help translating of software

#### Help out with translations {#HelpTranslating}

We use the [Transifex](https://www.transifex.com/) project to translate the user interface. You can sign up and get started at the [thebutterflyeffect](https://www.transifex.com/Magic/thebutterflyeffect/) page.

Note that levels and the UI have different translation mechanisms, but both are handled through Transifex.

#### Help out with new or improved levels

If you have ideas for levels, we'd love to hear from you!

Probably the easiest way to let us know about your ideas, is to create a login on the [GitHub website](https://github.com/kaa-ching/tbe) and create an issue for TBE. We'll work with you to make your ideas happen. Note that _Milestone B_ will feature a level editor again, so you can design your own levels!

#### Add more code

If you are a C++ programmer, we'd love to hear from you - there's always more things that can be coded!

Create a login on the [GitHub website](https://github.com/kaa-ching/tbe), clone the repo and start hacking. We love patches and pull requests!

#### Found any bugs? Or you have feedback? {#BugsFeedback}

Oh boy. Does TBE still contain bugs?

Please log in to GitHub and [file an issue](https://github.com/kaa-ching/tbe/issues) or contact lead developer [kaa-ching](https://github.com/kaa-ching) by e-mail.
