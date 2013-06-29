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

#include "TreeFilterProxyModel.h"
#include "TreeModel.h"
#include "TreeItem.h"

bool TreeFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &index) const
{
    //To apply when a subnodes/leafs editor will be done
    /**
    if (index.isValid())
    {
        TreeModel *model = dynamic_cast<TreeModel*>(sourceModel());

        if (model)
        {
            TreeItem *item = model->itemFromIndex(index);

            if (item and item->child(source_row)->hasChildren())
                return true;
        }

    }
	else if (source_row == 0)
		return true;

    return false;
    **/
    Q_UNUSED(source_row);
    Q_UNUSED(index);
    return true;
}
