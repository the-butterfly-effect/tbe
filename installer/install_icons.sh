#/bin/sh

# find the directory this script resides in - should be
# the same location the .desktop file and the .png file are
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ICON=sourceforge.net-tbe.desktop
sed -i "s:%TBE%:$DIR:g" $DIR/$ICON

# and let's install a menu item and a desktop item
echo "installing icons in Start Menu and Desktop..."
xdg-desktop-menu install $DIR/$ICON
xdg-desktop-icon install $DIR/$ICON
