#/bin/sh

# find the directory this script resides in - should be
# the same location the .desktop file and the .png file are
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
sed -i 's"%TBE%"$DIR"g' sf-tbe.desktop

# and let's install a menu item and a desktop item
xdg-desktop-menu install $DIR/sourceforge.net-tbe.desktop
xdg-desktop-icon install $DIR/sourceforge.net-tbe.desktop
