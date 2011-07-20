// This test code uses the QTestLib test harness
// to test the ImageRendererStore and ImageRenderer iplementations

#include <QtTest/QtTest>
#include "ImageStore.h"

#include <QImage>
#include <QPainter>

extern ImageRendererStore* theImageRendererStorePtr;

int theVerbosity = 4;

const char* ASCII(const QString& aQString)
{
	static char myString[256];
	strncpy(myString, aQString.toAscii().constData(), 255);
	return myString;
}


class TestImageRendererStore : public QObject
{
	Q_OBJECT

private:

private slots:
// a slot initTestCase() will be called before anything else
// a slot init() will be called before EACH test
// a slot cleanup() will be called after EACH test
// a slot cleanupTestCase() will be called once ALL tests are done


	// #########################################################################
	// ######################### readUntilCharacter ############################
	// #########################################################################

	/// how does readUntilCharacter() handle bad strings?
	void checkSingleton()
	{
		QVERIFY(theImageRendererStorePtr == NULL);
		ImageRenderer* myIRPtr = ImageRendererStore::getImageRenderer("hello-nonexisting-file",true);
		QVERIFY(theImageRendererStorePtr != NULL);
		QVERIFY(myIRPtr == NULL);
	}

	void checkLocalPng()
	{
		// obtain the first ImageRenderer, it should exist
		ImageRenderer* myIRPtr1 = ImageRendererStore::getImageRenderer("LocalPng", true);
		QVERIFY(myIRPtr1 != NULL);
		// if we retrieve it again, it should be the same pointer (cached!)
		ImageRenderer* myIRPtr2 = ImageRendererStore::getImageRenderer("LocalPng", true);
		QVERIFY(myIRPtr1 == myIRPtr2);

		// And perform render tests here...
		QImage myImage(200, 100, QImage::Format_RGB32);
		uint myBackground = QColor(77,88,99).rgb();
		myImage.fill(myBackground);
		QCOMPARE(myImage.pixel(111,23), myBackground);

		QPainter myPainter;
		myPainter.begin(&myImage);
		myPainter.setRenderHint(QPainter::Antialiasing);
		myIRPtr1->render(&myPainter, myPainter.viewport());
		myPainter.end();

		QVERIFY(myImage.pixel(111,23) != myBackground);
	}

	void checkNotFound()
	{
	  // if we can access the NotFound file,
	  // we should get a valid renderer back...
	  ImageRenderer* myIRPtr1 = ImageRendererStore::getImageRenderer("hello-nonexisting-file", false);
	  QVERIFY(myIRPtr1 != NULL);
	  ImageRenderer* myIRPtr2 = ImageRendererStore::getImageRenderer("NotFound", true);
	  QVERIFY(myIRPtr2 != NULL);
	  QVERIFY(myIRPtr1 == myIRPtr2);
	}

};


QTEST_MAIN(TestImageRendererStore)
#include "ImageRendererStore-tester.moc"
