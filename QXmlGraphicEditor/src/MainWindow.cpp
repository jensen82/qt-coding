/****************************************************************************************
 ** QXmlGraphicReader is an application to open, edit and save XML configuration files.
 ** Copyright (C) 2013  Francesc Martinez <es.linkedin.com/in/cescmm/en>
 **
 ** This library is free software; you can redistribute it and/or
 ** modify it under the terms of the GNU Lesser General Public
 ** License as published by the Free Software Foundation; either
 ** version 2.1 of the License, or (at your option) any later version.
 **
 ** This library is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 ** Lesser General Public License for more details.
 **
 ** You should have received a copy of the GNU Lesser General Public
 ** License along with this library; if not, write to the Free Software
 ** Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 ***************************************************************************************/

#include <QFileDialog>
#include <QTreeView>
#include <QScrollArea>
#include <QTableWidget>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QSplitter>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "TreeModel.h"
#include "TreeItem.h"
#include "FindDialog.h"
#include "AttributeWidget.h"
#include "TreeFilterProxyModel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    addAction(ui->actionFind);
    addAction(ui->action_Quit);
    connect(ui->actionFind, SIGNAL(triggered()), this, SLOT(showFindDialog()));
    connect(ui->actionNew_File, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(ui->action_Open_File, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->action_Save, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->action_Close_File, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(ui->actionAdd_Node, SIGNAL(triggered()), this, SLOT(insertNode()));
    connect(ui->actionAdd_Subnode, SIGNAL(triggered()), this, SLOT(insertSubnode()));
    connect(ui->action_Remove_Node, SIGNAL(triggered()), this, SLOT(removeNode()));
    connect(ui->actionAdd_Attribute, SIGNAL(triggered()), this, SLOT(addAttribute()));
    connect(ui->actionAbout_QXmlGraphicEditor, SIGNAL(triggered()), this, SLOT(aboutThis()));
    connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));

    horSplitter = new QSplitter(Qt::Horizontal, this);
    treeView = new QTreeView(horSplitter);
    scrollArea = new QScrollArea(horSplitter);
    tableWidget = new QTableWidget(horSplitter);
    connect(tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(selectTreeItem()));

    horSplitter->addWidget(treeView);
    horSplitter->addWidget(scrollArea);
    horSplitter->addWidget(tableWidget);
    horSplitter->setStretchFactor(0,1);
    horSplitter->setStretchFactor(1,4);
    horSplitter->setStretchFactor(2,1);

    treeView->setHeaderHidden(true);
    //treeView->setEditTriggers(QTreeView::NoEditTriggers);

    tableWidget->setSelectionMode(QTableWidget::SingleSelection);
    tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    tableWidget->setVisible(false);
    tableWidget->setColumnHidden(4, true);
    tableWidget->horizontalHeader()->setHidden(true);
    tableWidget->verticalHeader()->setHidden(true);

    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(showInfo(QModelIndex)));

    widget = NULL;
    gridLayout = NULL;
    model = NULL;

    //treeView->show();

    setCentralWidget(horSplitter);

    QString title = APP_NAME;
    title += VERSION;
    setWindowTitle(title);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    bool createNew = true;

    if (model)
    {
        int confirm = QMessageBox::question(this, tr("Close current file"), tr("Are you sure to close the current file?"),
                                            QMessageBox::Ok | QMessageBox::Cancel);

        if (confirm == QMessageBox::Cancel)
            createNew = false;
    }

    if (createNew)
    {
        closeFile();

        ui->action_Save->setEnabled(true);
        ui->action_Close_File->setEnabled(true);
        ui->actionAdd_Attribute->setEnabled(true);
        ui->actionAdd_Node->setEnabled(true);
        ui->actionAdd_Subnode->setEnabled(true);
        ui->action_Remove_Node->setEnabled(true);
        ui->actionFind->setEnabled(true);

        model = new TreeModel();
        connect(model, SIGNAL(modelModified()), this, SLOT(setNewModel()));

        TreeItem *rootItem = new TreeItem("[Root node]");
        model->setItem(0, rootItem);

        filterModel = new TreeFilterProxyModel();
        filterModel->setSourceModel(model);
        treeView->setModel(filterModel);

        QString title = APP_NAME;
        title += VERSION;
        setWindowTitle(title + " - File not save!");
    }
}

void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.xml)"));

    if (fileName != "")
    {
        closeFile();

        ui->action_Save->setEnabled(true);
        ui->action_Close_File->setEnabled(true);
        ui->actionAdd_Attribute->setEnabled(true);
        ui->actionAdd_Node->setEnabled(true);
        ui->actionAdd_Subnode->setEnabled(true);
        ui->action_Remove_Node->setEnabled(true);
        ui->actionFind->setEnabled(true);

        model = new TreeModel();
        connect(model, SIGNAL(modelModified()), this, SLOT(setNewModel()));

        readXml(fileName);

        filterModel = new TreeFilterProxyModel();
        filterModel->setSourceModel(model);
        treeView->setModel(filterModel);

        QString title = APP_NAME;
        title += VERSION;
        setWindowTitle(title + " - " + fileName);
    }
}

void MainWindow::readXml(const QString &_fileName)
{
    QFile file(_fileName);
    QString xmlContent = "";

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        xmlContent = file.readAll();
        file.close();
    }

    model->fillModel(xmlContent);
}

void MainWindow::closeFile()
{
    //Deleting model and cleaning QTreeView
    treeView->setModel(0);
    if (model != NULL)
    {
        delete model;
        model = NULL;
    }

    //Deleting scroll widget and cleaning QSrollArea
    if (widget != NULL)
    {
        delete widget;
        widget = NULL;
    }

    //Hidding QTableWidget
    tableWidget->setHidden(true);

    ui->action_Save->setDisabled(true);
    ui->action_Close_File->setDisabled(true);
    ui->actionAdd_Attribute->setDisabled(true);
    ui->actionAdd_Node->setDisabled(true);
    ui->actionAdd_Subnode->setDisabled(true);
    ui->action_Remove_Node->setDisabled(true);
    ui->actionFind->setDisabled(true);

    QString title = APP_NAME;
    title += VERSION;
    setWindowTitle(title);
}

void MainWindow::saveFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Files (*.xml)"));

    if (fileName != "")
    {
        model->writeXml(fileName);

        QString title = APP_NAME;
        title += VERSION;
        setWindowTitle(title + " - " + fileName);
    }
}

void MainWindow::setNewModel()
{
    treeView->reset();
	treeView->activateWindow();
	treeView->setFocus(Qt::OtherFocusReason);
    treeView->selectionModel()->setCurrentIndex(selectedIndex, QItemSelectionModel::ClearAndSelect);
}

void MainWindow::showInfo(const QModelIndex &index)
{
    selectedIndex = index;
	QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(index);
	TreeItem *item = model->itemFromIndex(originalIndex);

    if (widget != NULL)
    {
        delete widget;
        widget = NULL;
    }

    if (item->hasChildren())
	{
        widget = new QWidget();
        gridLayout = new QGridLayout(widget);

		TreeItem *child;
		for (int i = 0; i < item->rowCount(); i++)
		{
            child = item->child(i);

			if (!child->hasChildren())
			{
				gridLayout->addWidget(new QLabel(child->getName(), widget),i,0);

				QString defValue;
                if (child->getType().toString().toLower() == "enum" or child->getType().toString().toLower() == "enum_text" or child->getType().toString().toLower() == "list")
				{
					QComboBox *cbList = new QComboBox(widget);
					cbList->addItem("<Default>");
					cbList->addItems(child->getAvailableValuesList());

                    QString value = "<Default>";
                    if (child->getValue() != "")
                        value = child->getValue();

                    defValue = child->getDefaultValue().toString();
                    cbList->setCurrentIndex(cbList->findText(value));
                    cbList->setToolTip(child->getDescription().toString());
                    gridLayout->addWidget(cbList,i,1);
                    connect(cbList, SIGNAL(currentIndexChanged(int)), this, SLOT(saveNewData()));
                }
                else if (child->getType().toString().toLower() == "bool" or child->getType().toString().toLower() == "boolean")
                {
                    QComboBox *cbBool = new QComboBox(widget);
                    QStringList items;
                    items.append("<Default>");
                    items.append("TRUE");
                    items.append("FALSE");
                    cbBool->addItems(items);

                    QString strValue = "<Default>";

                    if (child->getValue() != "")
                    {
                        bool value = false;
                        value = QVariant(child->getValue()).toBool();
                        if (value)
                            strValue = "TRUE";
                        else
                            strValue = "FALSE";
                    }

					defValue = child->getDefaultValue().toString();
					if (defValue == "1")
						defValue = "TRUE";
					else
						defValue = "FALSE";
					cbBool->setCurrentIndex(cbBool->findText(strValue));
                    cbBool->setToolTip(child->getDescription().toString());
					gridLayout->addWidget(cbBool,i,1);
					connect(cbBool, SIGNAL(currentIndexChanged(int)), this, SLOT(saveNewData()));
				}
				else
				{
					QLineEdit *lineEdit = new QLineEdit("<Default>",widget);
                    lineEdit->setToolTip(child->getDescription().toString());
					QString value = child->getValue();

					if (value != "")
						lineEdit->setText(value);

					defValue = child->getDefaultValue().toString();
					connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(saveNewData()));
					gridLayout->addWidget(lineEdit,i,1);
				}
				if (defValue.size() > 0)
				{
					defValue = "(Default=" + defValue + ")";
					QLabel *labelDefault = new QLabel(defValue);
					gridLayout->addWidget(labelDefault,i,2);
				}

                QLabel *labelID = new QLabel(child->getUID());
                labelID->setVisible(false);
                gridLayout->addWidget(labelID,i,3);
            }
        }
        widget->setLayout(gridLayout);

		scrollArea->setWidget(widget);
	}
}

void MainWindow::fillTableView(const QString &textToFind, const QStringList &filters)
{
    if (model != NULL)
    {
        tableWidget->clear();
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(4);
        tableWidget->setColumnHidden(3, true);

        QList<TreeItem*> list = model->getFilteredItemList(textToFind, filters);

        for (int i = 0; i < list.size(); i++)
        {
            tableWidget->insertRow(i);
            tableWidget->setItem(i, 0, new QTableWidgetItem(list.at(i)->getName()));
            tableWidget->setItem(i, 1, new QTableWidgetItem(list.at(i)->getValue()));
            tableWidget->setItem(i, 2, new QTableWidgetItem(list.at(i)->parent()->getName()));
            tableWidget->setItem(i, 3, new QTableWidgetItem(list.at(i)->getUID()));
        }

        tableWidget->setVisible(true);
    }
}

void MainWindow::selectTreeItem()
{
    QString selectedUID = tableWidget->item(tableWidget->currentRow(), 3)->text();
    QModelIndex index = model->getIndexByUID(selectedUID);
    QModelIndex modelIndex = filterModel->mapFromSource(index);

    if (modelIndex.isValid())
    {
        treeView->selectionModel()->setCurrentIndex(modelIndex, QItemSelectionModel::ClearAndSelect);
        showInfo(modelIndex);

        //Set the focus over the value editable widget
        for (int i = 0; i < gridLayout->rowCount(); i++)
        {
            QWidgetItem *widgetItem = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,1));
            QWidgetItem *widgetUid = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,3));
            QLabel *label = dynamic_cast<QLabel*>(widgetItem->widget());

            if (widgetUid and label and widgetItem)
            {
                if (label->text() == selectedUID)
                {
                    widget->activateWindow();
                    widgetItem->widget()->setFocus(Qt::OtherFocusReason);
                }
            }
        }
    }
}

void MainWindow::showFindDialog()
{
    if (model)
    {
        find = new FindDialog();
        connect(find, SIGNAL(findSignal(QString,QStringList)), this, SLOT(fillTableView(QString,QStringList)));
        find->show();
    }
}

void MainWindow::insertNode()
{
    if (model)
    {
        QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
        QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);

        if (originalIndex.isValid())
        {
            TreeItem *item = new TreeItem("Node", "");
            TreeItem *brother = model->itemFromIndex(originalIndex);
            if (brother->parent() != NULL)
                brother->parent()->appendRow(item);
        }
    }
}

void MainWindow::insertSubnode()
{
    if (model)
    {
        QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
        QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);

        if (originalIndex.isValid())
        {
            TreeItem *item = new TreeItem("Subnode", "");
            TreeItem *parent = model->itemFromIndex(originalIndex);
            parent->appendRow(item);
        }
    }
}

void MainWindow::removeNode()
{
    if (model)
    {
        QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
        QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);

        if (originalIndex.isValid())
            model->removeRow(originalIndex.row(), originalIndex.parent());
    }
}

void MainWindow::addAttribute()
{
    if (model)
    {
        QModelIndex currentIndex = treeView->selectionModel()->currentIndex();
        QModelIndex originalIndex = dynamic_cast<TreeFilterProxyModel*>(treeView->model())->mapToSource(currentIndex);

        if (originalIndex.isValid())
        {
            TreeItem *item = model->itemFromIndex(originalIndex);
            attWidget = new AttributeWidget(item);
            attWidget->show();
        }
    }
}

void MainWindow::saveNewData()
{
    QModelIndex sourceIndex = filterModel->mapToSource(treeView->selectionModel()->currentIndex());
    model->saveNewData(widget, sourceIndex);
}

void MainWindow::aboutThis()
{
    QString text = "<p>QXmlGraphicEditor is licenced under GNU LGPL version 2.1. It is appropiate "
                   "for development of software where you want to use free software in commercial. "
                   "You can copy, modify or redistribute this software under same terms that are "
                   "exposed here and with no more premissions than the GNU LGPL version 2.1 especifies. "
                   "Related with described before, you must recognize the original author in all "
                   "derivative works.</p><p>For more information go to "
                   "<a href='http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html'target='_blank'>GNU.org</a>.</p>";
    QMessageBox::about(this, "About QXmlGraphicEditor", text);
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}
