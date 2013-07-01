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

#include "TreeItem.h"

TreeItem::TreeItem(const QString &name, const QString &value) : QStandardItem(name)
{
    m_value = value;
    uid = QUuid::createUuid();
}

TreeItem::TreeItem(const TreeItem &item) : QStandardItem(item.getName())
{
    m_value = item.m_value;
    m_storeValue = item.m_storeValue;
    m_attributes = item.m_attributes;
    uid = QUuid::createUuid();
}

TreeItem::TreeItem(TreeItem *item) : QStandardItem(item->getName())
{
    m_value = item->m_value;
    m_storeValue = item->m_storeValue;
    m_attributes = item->m_attributes;
    uid = QUuid::createUuid();
}

QVariant TreeItem::getType() const
{
    if (m_attributes.contains("TYPE"))
        return m_attributes.value("TYPE");
    else if (m_attributes.contains("Type"))
        return m_attributes.value("Type");
    else if (m_attributes.contains("type"))
        return m_attributes.value("type");
    else
        return QVariant();
}

QVariant TreeItem::getDescription() const
{
    if (m_attributes.contains("Description"))
        return m_attributes.value("Description");
    else if (m_attributes.contains("DESCRIPTION"))
        return m_attributes.value("DESCRIPTION");
    else if (m_attributes.contains("description"))
        return m_attributes.value("description");
    else
        return QVariant();
}

QVariant TreeItem::getDefaultValue() const
{
    if (m_attributes.contains("Default"))
        return m_attributes.value("Default");
    else if (m_attributes.contains("DEFAULT"))
        return m_attributes.value("DEFAULT");
    else if (m_attributes.contains("default"))
        return m_attributes.value("default");
    else
        return QVariant();
}

QStringList TreeItem::getAvailableValuesList() const
{
    if (m_attributes.contains("LIST"))
        return m_attributes.value("LIST").toString().split(";");
    else if (m_attributes.contains("List"))
        return m_attributes.value("List").toString().split(";");
    else if (m_attributes.contains("list"))
        return m_attributes.value("list").toString().split(";");
    else
        return QStringList();
}

bool TreeItem::existAttribute(const QString &attName) const
{
    bool exists = false;

    if (m_attributes.key(attName) != "")
        exists = true;

    return exists;
}
