#/bin/sh

# find the directory this script resides in - should be
# the same location the .desktop file and the .png file are
DIR="$( cd "$( dirname "$0" )" && pwd )"
ICON=sourceforge.net-tbe.desktop

# this statement should not have any effect:
sed -i 's"%TBE%"$DIR"g' $DIR/$ICON

# and let's install a menu item and a desktop item
echo "uninstalling icons in Start Menu and Desktop..."
xdg-desktop-menu uninstall $DIR/$ICON
xdg-desktop-icon uninstall $DIR/$ICON
