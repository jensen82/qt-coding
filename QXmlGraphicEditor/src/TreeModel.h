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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>

class QDomNode;
class QXmlStreamWriter;
class TreeItem;

class TreeModel : public QStandardItemModel
{
    Q_OBJECT

    signals:
        void modelModified();

    public:
        explicit TreeModel(QObject *parent = 0);
        void fillModel(const QString &xmlContent);
        TreeItem *getRootItem() const { return rootItem; }
        void writeXml(const QString &fileName);
        QList<TreeItem*> getFilteredItemList(const QString &textToFind, const QStringList &filters);
        QModelIndex getIndexByUID(const QString &uuid);
        TreeItem * itemFromIndex(const QModelIndex &index) const;

    public slots:
        void saveNewData(QWidget *widgetContainer, const QModelIndex &parentIndex);

    private:
        TreeItem *rootItem;

        void recursiveRead(QDomNode dNode, TreeItem *item);
        void recursiveWrite(QXmlStreamWriter &xml, TreeItem *item);
        void recursiveSearch(const QString &textToFind, const QStringList &filters, TreeItem *item, QList<TreeItem *> &list);
        void recursiveUidSearch(const QString &uuid, TreeItem *item, bool found, QModelIndex &index);
};

#endif // TREEMODEL_H
