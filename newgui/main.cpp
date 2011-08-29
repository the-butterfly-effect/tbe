#include <QtGui/QApplication>
#include "MainWindow.h"

// the verbosity for all logging - by default defined at 4
// accepted values are 0 (no logging) - 6 (most logging)
// note that levels 5 and 6 affect playing
int theVerbosity = 4;

// this variable defines whether we are playing or a level editor
// switching to LevelEditor is possible through the "Edit" menu.
bool theIsLevelEditor = false;

// this variable defines whether in the level editor we are expecting collision
// detection to work or not - you want this on to align certain things
// TODO/FIXME: this should go into preferences or so at some point
bool theIsCollisionOn = false;

// this variable defines whether we are showing Box2D debugging info or not
// switching this option is possible through the theDrawDebugActionPtr in MainWindow
bool theDrawDebug = false;

// this variable defines whether DrawPolyOutline draws the polygons
// over the image
// switching this option is possible through the theDrawOutlineActionPtr in MainWindow
bool theDrawPolyOutline = false;


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}

const char* ASCII(const QString& aQString)
{
		static char myString[256];
		strncpy(myString, aQString.toAscii().constData(), 255);
		return myString;
}
