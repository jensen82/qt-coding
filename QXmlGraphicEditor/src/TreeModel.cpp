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

#include <QFile>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include "TreeModel.h"
#include "TreeItem.h"

TreeModel::TreeModel(QObject *parent) : QStandardItemModel(parent)
{
    rootItem = NULL;
}

void TreeModel::fillModel(const QString &xmlContent)
{
    QDomDocument doc;
    QDomElement root;
    QDomNode dNode;

    doc.setContent(xmlContent);
    root = doc.documentElement();
    dNode = root.firstChild();

    rootItem = new TreeItem(root.tagName());

    recursiveRead(dNode, rootItem);

    setItem(0, rootItem);
}

void TreeModel::recursiveRead(QDomNode dNode, TreeItem *item)
{
    do
    {
        int totalOfChilds = dNode.childNodes().size();

        if (dNode.nodeType() != QDomNode::CommentNode)
        {
            if (totalOfChilds == 0)
            {
                if (dNode.nodeType() == QDomNode::TextNode)
                    item->setValue(dNode.nodeValue());
                else
                {
                    TreeItem *subItem = new TreeItem(dNode.nodeName());

                    for (int i = 0; i < dNode.attributes().size(); i++)
                        subItem->addAttribute(dNode.attributes().item(i).nodeName(), dNode.attributes().item(i).nodeValue());

                    item->appendRow(subItem);

                }
            }
            else
            {
                TreeItem *item2 = new TreeItem(dNode.nodeName());

                for (int i = 0; i < dNode.attributes().size(); i++)
                    item->addAttribute(dNode.attributes().item(i).nodeName(), dNode.attributes().item(i).nodeValue());

                for (int i = 0; i < totalOfChilds; i++)
                    if (dNode.childNodes().size() > 0 and i == 0)
                        recursiveRead(dNode.childNodes().at(i), item2);

                item->appendRow(item2);
            }
        }

        dNode = dNode.nextSibling();
    }
    while (!dNode.isNull());
}

void TreeModel::saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex)
{
    TreeItem *m_parent = itemFromIndex(parentIndex);

	if (m_parent != NULL)
	{
		int totalChilds = m_parent->childCount();
		QGridLayout *gridLayout = dynamic_cast<QGridLayout*>(widgetContainer->layout());

		for (int i = 0; i < gridLayout->rowCount(); i++)
		{
            QWidgetItem *widgetItem = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,3));
			if (widgetItem)
			{
				QLabel *label = dynamic_cast<QLabel*>(widgetItem->widget());
				QWidgetItem *widgetItem2 = dynamic_cast<QWidgetItem*>(gridLayout->itemAtPosition(i,1));

				if (label and widgetItem2)
				{
					for (int j = 0; j < totalChilds; j++)
					{
						if (label->text() == m_parent->child(j)->getUID())
						{
							QLineEdit *lineEdit = qobject_cast<QLineEdit*>(widgetItem2->widget());
							QComboBox *comboBox = qobject_cast<QComboBox*>(widgetItem2->widget());
							if (lineEdit)
								m_parent->child(j)->setValue(lineEdit->text());
							else if (comboBox)
                            {
                                QString valAux = "";
                                if (comboBox->currentText() == "TRUE")
                                    valAux = "1";
                                else if (comboBox->currentText() == "FALSE")
                                    valAux = "0";

                                m_parent->child(j)->setValue(valAux);
                            }

							TreeItem *item = new TreeItem(m_parent->child(j));
							m_parent->removeRow(j);
							m_parent->insertRow(j, item);
						}
					}
				}
			}
		}

		emit modelModified();
	}
}

void TreeModel::writeXml(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        recursiveWrite(xmlWriter, rootItem);

        xmlWriter.writeEndDocument();
        file.close();
    }
}

void TreeModel::recursiveWrite(QXmlStreamWriter &xml, TreeItem *item)
{
    if (item->hasChildren())
    {
        xml.writeStartElement(item->getName());
        QList<QString> keyAttributes = item->getAttributes().keys();
        for (int i = 0; i < keyAttributes.count(); i++)
            xml.writeAttribute(keyAttributes.at(i), item->getAttribute(keyAttributes.at(i)).toString());

        for (int i = 0; i < item->childCount(); i++)
            recursiveWrite(xml, item->child(i));

        xml.writeEndElement();
    }
    else
    {
        if (item->getValue() == "")
        {
            xml.writeEmptyElement(item->getName());
            QList<QString> keyAttributes = item->getAttributes().keys();
            for (int i = 0; i < keyAttributes.count(); i++)
                xml.writeAttribute(keyAttributes.at(i), item->getAttribute(keyAttributes.at(i)).toString());
        }
        else
            xml.writeTextElement(item->getName(), item->getValue());
    }
}

QList<TreeItem*> TreeModel::getFilteredItemList(const QString &textToFind, const QStringList &filters)
{
    QList<TreeItem*> list;

    recursiveSearch(textToFind, filters, rootItem, list);

    return list;
}

void TreeModel::recursiveSearch(const QString &textToFind, const QStringList &filters, TreeItem *item, QList<TreeItem*> &list)
{
    QString itName = item->getName();
    QString itValue = item->getValue();
    QString text = textToFind;

    if (!filters.contains("CASE_SENSITIVE"))
    {
        text = textToFind.toLower();
        itName = itName.toLower();
        itValue = itValue.toLower();
    }

    if ((itName == text and filters.contains("TAGS")) ||
        (itValue == text and filters.contains("VALUES")) ||
        (item->getAttribute(textToFind).toString().size() > 0 and filters.contains("ATTRIBUTES")))
    {
        list.append(item);
    }
    if (item->hasChildren())
        for (int i = 0; i < item->rowCount(); i++)
            recursiveSearch(textToFind, filters, item->child(i), list);
}

QModelIndex TreeModel::getIndexByUID(const QString &uuid)
{
    QModelIndex index;
    bool found = false;

    recursiveUidSearch(uuid, rootItem, found, index);

    return index;
}

void TreeModel::recursiveUidSearch(const QString &uuid, TreeItem *item, bool found, QModelIndex &index)
{
    if (item->getUID() == uuid)
    {
        index = indexFromItem(item->parent());
        found = true;
    }
    else if (item->hasChildren())
        for (int i = 0; i < item->rowCount() and !found; i++)
            recursiveUidSearch(uuid, item->child(i), found, index);
}

TreeItem * TreeModel::itemFromIndex(const QModelIndex &index) const
{
    return dynamic_cast<TreeItem*>(QStandardItemModel::itemFromIndex(index));
}
