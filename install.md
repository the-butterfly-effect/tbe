---
layout: page
title: Installation instructions
---

## Mageia

Thanks to packager/developer _AlexL_, TBE is available directly from within Mageia!

## RPM-based distributions

Every RPM-based distribution allows to use the `rpm` command to install the RPM package like this (make sure to supply the correct file name):
```
    rpm -ivh thebutterflyeffect.rpm
```

If, however, RPM detects that there are missing dependencies (i.e. the game needs other libraries or packages that are not yet installed), the resolution depends on the version of Linux you are using:

 * Fedora / CentOS: use `yum` to install, the yum tool itself will figure out missing dependencies and install them.
 * Opensuse: use `zypper` to install, the zypper tool itself will figure out missing dependencies and will download & install them:

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

<p class="bg-warning">This is currently being worked on.</p>

 - [ ] discuss dependencies
 - [ ] discuss executable rights
 - [ ] discuss where to install

## Windows

Download the installer, double click the installer and follow the steps.
The game will be installed and is available in the start menu. The start menu will also show an `uninstall` icon if you ever want to remove the game again.


