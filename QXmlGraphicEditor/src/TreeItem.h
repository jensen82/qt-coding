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

#define and &&
#define or ||

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QUuid>
#include <QTime>
#include <QCryptographicHash>
#include <QStandardItem>
#include <QTableWidgetItem>

class TreeItem : public QStandardItem
{
    public:
        TreeItem(const QString &name, const QString &value = "");
		TreeItem(const TreeItem &item);
		TreeItem(TreeItem *item);
        QString getUID() const { return uid.toString(); }
        QString getName() const { return QStandardItem::text(); }
        void setValue(const QString &value) { m_value = value; }
        QString getValue() const { return m_value; }
        void setStoreValue(bool store) { m_storeValue = store; }
        bool canStoreValue() const { return m_storeValue; }
        void addAttribute(const QString &attName, const QVariant &attValue) { m_attributes.insert(attName,attValue); }
        void setAttributes(const QMap<QString,QVariant> &attributes) { m_attributes = attributes; }
        QVariant getAttribute(const QString &attName) const { return m_attributes.value(attName); }
        QMap<QString,QVariant> getAttributes() const { return m_attributes; }
        int getAttributesCount() { return m_attributes.size(); }
        QStringList getAvailableValuesList() const;
        QVariant getType() const;
        QVariant getDescription() const;
        QVariant getDefaultValue() const;
        bool existAttribute(const QString &attName) const;
        TreeItem * child(int row) const { return dynamic_cast<TreeItem*>(QStandardItem::child(row,0)); }
        int childCount() const { return rowCount(); }
        TreeItem * parent() const { return dynamic_cast<TreeItem*>(QStandardItem::parent()); }
        int row() const { return QStandardItem::row(); }

    private:
        QUuid uid;
        QString m_value;
        bool m_storeValue;
        QMap<QString,QVariant> m_attributes;
};

#endif // TREEITEM_H
