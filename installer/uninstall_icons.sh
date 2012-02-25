#/bin/sh

# find the directory this script resides in - should be
# the same location the .desktop file and the .png file are
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
sed -i 's"%TBE%"$DIR"g' sf-tbe.desktop

# and let's remove our menu item and desktop item
xdg-desktop-menu uninstall $DIR/sourceforge.net-tbe.desktop
xdg-desktop-icon uninstall $DIR/sourceforge.net-tbe.desktop