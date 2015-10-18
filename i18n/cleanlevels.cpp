#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDomDocument>
#include <QApplication>
#include <QObject>


static const char* theLevelInfoString = "levelinfo";
	static const char* theLevelDescriptionString  = "description";
	static const char* theLevelNameString         = "title";
	static const char* thePredefinedString = "predefined";
		   const char* theObjectString     = "object";
		   const char* thePropertyString   = "property";
		   const char* theToolTipString    = "tooltip";
static const char* theToolboxString = "toolbox";

const char* theWidthAttributeString     = "width";
const char* theHeightAttributeString    = "height";
const char* theXAttributeString         = "X";
const char* theYAttributeString         = "Y";
const char* theAngleAttributeString     = "angle";
const char* theTypeAttributeString      = "type";
const char* theIDAttributeString        = "ID";
const char* theIsFailAttributeString    = "isFail";

void deleteFromDOM(
		QDomNode& aNode,
		const QString& aTagString)
{
	QDomElement myE = aNode.firstChildElement(aTagString);
	while (myE.isNull()==false)
	{
		QDomElement myNext = myE.nextSiblingElement(aTagString);
		if (myE.attribute("lang","-") != "-")
		{
			QString msg = QString("found something to erase: %1, %2\n")
				.arg(aTagString).arg(myE.attribute("lang","-"));
			printf("%s", msg.toAscii().constData());
			aNode.removeChild(myE);
		}
		myE = myNext;
	}
}


int main(int argc, char *argv[])
{
	//** init Qt (graphics toolkit) - www.qtsoftware.com
	QApplication app(argc, argv);

	if (argc <2)
	{
		printf("needs 1 argument: %s infile\n", argv[0]);
		exit(3);
	}
	
	QString aFileName(argv[1]);
	QString aSaveFileName = aFileName;
	
	QString myErrorMessage = QString("Cannot read file '%1'").arg(aFileName);
	QDomDocument myDocument("mydocument");

	QDomNode myNode, q;
	QDomNode mySceneNode;
	QDomElement myElement;
	QDomElement myDocElem;
	QDomNamedNodeMap myNodeMap;
	QString myResult;

	QFile myFile(aFileName);
	if (!myFile.open(QIODevice::ReadOnly))
	{
		myErrorMessage = QString("Cannot read file '%1'").arg(aFileName);
		goto not_good;
	}

	if (!myDocument.setContent(&myFile))
	{
		myErrorMessage = QString("Cannot parse file - not valid XML?");
		goto not_good;
	}
	myFile.close();

	myDocElem = myDocument.documentElement();

	//
	// parse the Level Info section
	//
	myErrorMessage = QString("Parsing '%1' section failed: ").arg(theLevelInfoString);
	myNode=myDocElem.firstChildElement(theLevelInfoString);
	if (myNode.isNull())
		goto not_good;
	
	deleteFromDOM(myNode, theLevelNameString);
	deleteFromDOM(myNode, theLevelDescriptionString);

	//
	// parse the Toolbox section
	//
	myErrorMessage = QString("Parsing '%1' section failed: ").arg(theToolboxString);
	myNode=myDocElem.firstChildElement(theToolboxString);
	for (q=myNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// remove the translations
		deleteFromDOM(q, "name");
	}

	//
	// Parse the predefined section
	//
	mySceneNode=myDocElem.firstChildElement("scene");
	myErrorMessage = QString("Parsing '%1' section failed: ").arg(thePredefinedString);
	myNode = mySceneNode.firstChildElement(thePredefinedString);

	for (q=myNode.firstChild(); !q.isNull(); q=q.nextSibling())
	{
		// take out the postits
//             <object width="0.440" X="1.289" Y="2.976" height="0.440" type="PostIt" angle="0.000" >
//                 <property key="page1" >I hate Angry Birds!</property>
//                 <property key="page1_nl" >Ik haat die Angry Birds!</property>
//             </object>
		if (q.hasChildNodes()==true)
		{
			// to parse:   <property key="texture">used_wood_bar</property>
			QDomElement i;
			for (i=q.firstChildElement(); !i.isNull();)
			{
				QString myKey = i.attributes().item(0).nodeValue();
				QString myTValue = i.text();
				if (i.nodeName() == thePropertyString)
				{
// 					QString myS = QString("property: '%1'='%2'").arg(myKey).arg(myTValue);
// 					printf("%s\n", myS.toAscii().constData());
					
					if (myKey.contains("_"))
					{
						auto j=i.nextSiblingElement();
						q.removeChild(i);
						i=j;
// 						printf("DELETE IT\n");
						continue;
					}
				}
				if (i.nodeName() == theToolTipString)
				{
// 					QString myS = QString("tooltip: '%1'='%2'").arg(myKey).arg(myTValue);
// 					printf("%s\n", myS.toAscii().constData());
				}
				
				i=i.nextSiblingElement();
			}
		}

		// no need to do the tooltips
		if (q==myNode.lastChild())
			break;
	}

	{
		// success: we're going to write!
		QFile myFile(aSaveFileName);
		if (!myFile.open(QFile::WriteOnly | QFile::Text))
		{
			myErrorMessage = QString("Cannot write file '%1': %2.").arg(aSaveFileName, myFile.errorString());
			goto not_good;
		}
		const int IndentSize = 4;
		QTextStream myOut(&myFile);
		myDocument.save(myOut, IndentSize);
	}
	
	myErrorMessage = "everything went fine";
	
	// if goto not_good was called, we get here, too
	// theErrorMessage is set - let's return it!
not_good:
	printf("%s\n", myErrorMessage.toAscii().constData());
	return 0;
}
