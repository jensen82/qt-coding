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

#include <QModelIndex>
#include "AttributeWidget.h"
#include "ui_AttributeWidget.h"
#include "TreeItem.h"

AttributeWidget::AttributeWidget(TreeItem *item) : QWidget(), ui(new Ui::AttributeWidget)
{
    itemEditing = item;
    m_attributes = item->getAttributes();

    setAttribute(Qt::WA_DeleteOnClose);

    ui->setupUi(this);
    ui->twAttributes->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    connect(ui->pbAdd, SIGNAL(clicked()), this, SLOT(addAttributes()));
    connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(removeAttributes()));
    connect(ui->pbSave, SIGNAL(clicked()), this, SLOT(saveData()));
    connect(ui->twAttributes, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(modifyTable(QModelIndex)));

    setWindowTitle("Attributes of node: " + itemEditing->getName());
    addAction(ui->actionClose);

    if (m_attributes.size() > 0)
        fillAttTable();
}

AttributeWidget::~AttributeWidget()
{
    delete ui;
}

void AttributeWidget::fillAttTable()
{
    QList<QString> keyList = m_attributes.keys();

    for (int i = 0; i < m_attributes.size(); i++)
    {
        ui->twAttributes->insertRow(ui->twAttributes->rowCount());
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 0, new QTableWidgetItem(keyList.at(i)));
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 1, new QTableWidgetItem(m_attributes.value(keyList.at(i)).toString()));
    }
}

void AttributeWidget::addAttributes()
{
    if (ui->leName->text().size() != 0)
    {
        m_attributes.insert(ui->leName->text(), ui->leValue->text());
        ui->twAttributes->insertRow(ui->twAttributes->rowCount());
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 0, new QTableWidgetItem(ui->leName->text()));
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 1, new QTableWidgetItem(ui->leValue->text()));

        ui->leName->setText("");
        ui->leValue->setText("");
    }
}

void AttributeWidget::removeAttributes()
{
    int row = -1;
    QList<QTableWidgetItem*> itemList = ui->twAttributes->selectedItems();

    if (itemList.size() > 0)
    {
        row = itemList.at(0)->row();

        if (row >= 0)
        {
            QTableWidgetItem *item = ui->twAttributes->item(row,0);

            if (item != NULL)
            {
                m_attributes.remove(item->text());
                ui->twAttributes->removeRow(row);
            }
        }
    }
}

void AttributeWidget::saveData()
{
    itemEditing->setAttributes(m_attributes);

    close();
}

void AttributeWidget::modifyTable(const QModelIndex &index)
{
    if (!index.isValid())
    {
        ui->twAttributes->insertRow(ui->twAttributes->rowCount());
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 0, new QTableWidgetItem("[No Data]"));
        ui->twAttributes->setItem(ui->twAttributes->rowCount()-1, 1, new QTableWidgetItem("[No Data]"));
    }
}
