
#include "tbe_global.h"
#include "test.h"

Test::Test() 
	: QObject()
{
	QString myMsg = tr("Hello, world: it's time to rock!");
	printf("myMsg: '%s'\n", ASCII(myMsg));
}
