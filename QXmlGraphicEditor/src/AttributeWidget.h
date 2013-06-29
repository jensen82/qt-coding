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

#ifndef ATTRIBUTEWIDGET_H
#define ATTRIBUTEWIDGET_H

#include <QWidget>
#include <QVariant>
#include <QMap>

class QModelIndex;
class TreeItem;

namespace Ui
{
    class AttributeWidget;
}

class AttributeWidget : public QWidget
{
    Q_OBJECT
        
    signals:
        void newAttributes(QMap<QString,QVariant> attributes);

    public:
        explicit AttributeWidget(TreeItem *item);
        ~AttributeWidget();
        
    private:
        Ui::AttributeWidget *ui;
        QMap<QString,QVariant> m_attributes;
        TreeItem *itemEditing;

        void fillAttTable();

    private slots:
        void addAttributes();
        void removeAttributes();
        void saveData();
        void modifyTable(const QModelIndex &index);
};

#endif // ATTRIBUTEWIDGET_H
