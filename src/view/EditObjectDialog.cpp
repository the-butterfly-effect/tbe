#include "EditObjectDialog.h"

EditObjectDialog::EditObjectDialog(BaseObject* aBaseObjectPtr, QWidget *aParent)
		: QDialog(aParent, Qt::Tool), theBOPtr(aBaseObjectPtr)
{
	DEBUG1("EditObjectDialog::EditObjectDialog\n");
	ui.setupUi(this);

	theBOPtr = NULL;
	readFromObject(aBaseObjectPtr);

}

EditObjectDialog::~EditObjectDialog()
{
}

void EditObjectDialog::readFromObject(BaseObject* aBaseObjectPtr)
{
	if (aBaseObjectPtr==NULL)
		goto theEnd;

	ui.lineEditID->setText(aBaseObjectPtr->getID());
	ui.groupBox->setTitle(tr("Basic Properties for %1").arg(aBaseObjectPtr->getName()));
	//  TODO: QLabel *labelUnique;

	ui.spinBoxX     -> setValue(aBaseObjectPtr->getOrigCenter().x);
	ui.spinBoxY     -> setValue(aBaseObjectPtr->getOrigCenter().y);
	ui.spinBoxAngle -> setValue(aBaseObjectPtr->getOrigCenter().angle);
	ui.spinBoxWidth -> setValue(aBaseObjectPtr->getTheWidth());
	ui.spinBoxHeight-> setValue(aBaseObjectPtr->getTheHeight());

	//
	//	QTreeWidget *treeWidget;
	if (aBaseObjectPtr!=theBOPtr)
	{
		ui.treeWidget->clear();
		QStringList myAllPropertiesList = aBaseObjectPtr->theProps.getDefaultPropertyList();
		QStringList::iterator myI= myAllPropertiesList.begin();
		while (myI != myAllPropertiesList.end())
		{
			QString myKey = *myI;
			QString myValue = aBaseObjectPtr->theProps.getDefaultProperty(*myI);
			aBaseObjectPtr->theProps.propertyToString(*myI, &myValue);
			QTreeWidgetItem* myLine = new QTreeWidgetItem(ui.treeWidget);
			myLine->setText(0, myKey);
			myLine->setText(1, myValue);
			myI++;
		}
	}


theEnd:
	if (aBaseObjectPtr==NULL)
		setEnabled(false);
	else
	setEnabled(true);
	theBOPtr = aBaseObjectPtr;
	return;
}
