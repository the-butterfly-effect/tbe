# How to make a release

This page will discuss how to build a release of The Butterfly Effect.

## Building packages with dynamic library dependencies

GNU/Linux: just type:

 * `make clean`
 * `pushd i18n`
 * `./tbe_levels_i18n.sh`
 * `popd`
 * `make`
 * `make package`

This should result in 4 packages in the `build` directory:

 * `.deb`    <- for Debian and Ubuntu systems
 * `.rpm`    <- for Fedora, Red Hat, CentOS, Opensuse and Suse systems
 * `.sh`     <- for local installation (e.g. in your home directory) self-extracting archive
 * `.tar.gz` <- tarball for local installation (e.g. in your home directory)

For Mageia Linux, please refer to their package repository: it's in there!

## Building static packages
*TODO*

## Building installers
*TODO*

