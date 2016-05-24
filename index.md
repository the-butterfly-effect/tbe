---
layout: page
---

The Butterfly Effect is a free and open source game that uses realistic physics simulations to combine lots of simple mechanical elements to achieve a simple goal in the most complex way possible.

This is a fully playable game of alpha quality, well on its way to a shiny 1.0 release.

## Features

_The Butterfly Effect_ has been over five years in the making. That means that even though the game has been designed by hobbyists, the game is rich in features:

 * Many different objects: cola bottles, balloons, butterflies, domino stones, springs, dynamite, seesaws, penguins
 * Many different types of puzzles
 * Works on Linux, Windows and MacOX
 * Playable in English, Dutch, German, Russian, French, Danish, Swedish, Turkish, Ukrainian and several other languages. Note that not all languages are 100% [yet](#HelpTranslating)
 * Using vector graphics - all images always rendered at the highest quality
 * in Milestone B, there are roughly 70 levels. All levels are playable, but a few levels may need a bit more polish.

## Screenshots

<div class="row">
    <div class="col-md-offset-1 col-md-8">
        <div class="row">
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-angry-birds2.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-angry-birds2.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-draft-dialbforboom.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-draft-dialbforboom.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-draft-find-the-message.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-draft-find-the-message.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
        </div><!-- /.row -->
        <div class="row">
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-draft-save-the-butterfly.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-draft-save-the-butterfly.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-games-extreme-tux-racer.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-games-extreme-tux-racer.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
            <a class="col-sm-4" data-toggle="lightbox" data-gallery="screenshots" href="{{ "/images/lvl-picnic-picnic-1.png" | prepend: site.baseurl }}"><img src="{{ "/images/lvl-picnic-picnic-1.png" | prepend: site.baseurl }}" class="img-responsive" /></a>
        </div><!-- /.row -->
    </div><!-- /.col-md-off-set-1 col-md-8 -->
</div>

(the screenshots are clickable)

## Download

We have packages of the latest version available for the following Linux distributions and Windows:

If you need them, please refer to our [installation instructions](install). If you want to know what's most popular, please take a look at our [download statistics](http://www.somsubhra.com/github-release-stats/?username=the-butterfly-effect&repository=tbe).

| Operating System       | MD5SUM     | &nbsp;Download files&nbsp; | Notes |
|:-----------------------|:----------:|:--------------------------:|:------|
| Debian / Ubuntu / Mint | 2df5..b6c6 |  [tbe.deb](https://github.com/the-butterfly-effect/tbe/releases/download/v0.9.3.1/the-butterfly-effect.org-0.9.3.1-Linux.deb) |  |
| Fedora                 | n/a        | see [pkgs.org](http://pkgs.org/search//usr/share/applications/tbe.desktop)  | Fedora 21 and 22 |
| Opensuse               | afb6..5df1 |  [tbe.rpm](https://github.com/the-butterfly-effect/tbe/releases/download/v0.9.3.1/the-butterfly-effect.org-0.9.3.1-Linux.rpm) | Validated to work on Opensuse 42.1 |
| Arch Linux             |  n/a       |  n/a | if you want TBE in Arch, please [vote](https://aur.archlinux.org/packages/tbe/) |
| Mageia Linux           |  n/a       | see [pkgs.org](http://pkgs.org/search//usr/share/applications/tbe.desktop) | Mageia 5 |
| Other Linux            | 02d7..ccaf |  [tbe.sh](https://github.com/the-butterfly-effect/tbe/releases/download/v0.9.3.1/the-butterfly-effect.org-0.9.3.1-Linux.sh) | Self-extracting archive, make sure to install Qt5 5.2+ |
| Windows 7 or newer     | f257..1189 |  [tbe.exe](https://github.com/the-butterfly-effect/tbe/releases/download/v0.9.3.1/the-butterfly-effect.org-0.9.3.1-win32.exe) | Windows 7 or newer |
| MacOS X                | 0836..cc1e | El Captitan only:  | [tbe.zip](https://github.com/the-butterfly-effect/tbe/releases/download/v0.9.3.1/the-butterfly-effect.org-c98676323e380704a1f9b41a8337bc362b88f779-OSX.zip) |

_if you are a packager and want to add TBE to a distribution or build for another platform, do not hesitate to [contact the developers](#BugsFeedback)_

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

Probably the easiest way to let us know about your ideas, is to create a login on the [GitHub website](https://github.com/the-butterfly-effect/tbe) and create an issue for TBE. We'll work with you to make your ideas happen. Note that as of _Milestone B_ we have a level editor again, so you can design your own levels!

#### Add more code

If you are a C++ programmer, we'd love to hear from you - there's always more things that can be coded!

Create a login on the [GitHub website](https://github.com/the-butterfly-effect/tbe), clone the repo and start hacking. We love patches and pull requests!

#### Found any bugs? Or you have feedback? {#BugsFeedback}

Oh boy. Does TBE still contain bugs?

Please log in to GitHub and [file an issue](https://github.com/the-butterfly-effect/tbe/issues) or contact the developers [the-butterfly-effect](https://github.com/the-butterfly-effect) by e-mail.
