---
layout: page
title: Installation instructions
---

## Mageia

Thanks to packager/developer _AlexL_, TBE will be available directly from within Mageia 6! You can use the graphical `rpmdrake` tool or use the command line `urpmi` tool to install the package.

## RPM-based distributions

<p class="text-warning">As of mid-october 2015, we only supply the RPM for Opensuse 13.2 as one of the developers works on Opensuse. We will update this page once other packages catch up after the final release.</p>

Every RPM-based distribution allows to use the `rpm` command to install the RPM package like this (make sure to supply the correct file name):
```
    rpm -ivh thebutterflyeffect.rpm
```

If, however, RPM detects that there are missing dependencies (i.e. the game needs other libraries or packages that are not yet installed), the resolution depends on the version of Linux you are using:

 * Opensuse: use `zypper` to install the opensuse RPM, the zypper tool itself will figure out missing dependencies and will download & install them:
```
    sudo zypper install thebutterflyeffect.opensuse.rpm
```

Of course, you can also use graphical package managers.

The game should add itself to your start menu automatically.

## DEB-based distributions

Every DEB-based distribution, like _Debian_, _Ubuntu_ and _Linux Mint_ allow to use the `dpkg` command to install the _.deb_ file. However, that will likely not work because dpkg is not really good at dependencies.

There are essentially three options:

 * use graphical tools like synaptic
 * use a command line tool called `gdebi`:
   * `sudo apt-get update`
   * `sudo apt-get install gdebi`
   * `sudo gdebi thebutterflyeffect.deb`
 * use the self-extracting archive below.

## Self-extracting archive

<p class="text-warning">[This is currently being worked on.](https://github.com/the-butterfly-effect/tbe/issues/59)</p>

## Windows

Download the installer, double click the installer and follow the steps. The game will be installed and is available in the start menu. The start menu will also show an `uninstall` icon if you ever want to remove the game again.

## MacOSX

For now, please compile the game yourself. We plan to have packaging done before Christmas 2015.

