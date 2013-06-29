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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define APP_NAME "QXmlGraphicEditor "
#define VERSION "1.0"

#include <QMainWindow>
#include <QModelIndex>

class QTreeView;
class QScrollArea;
class QTableWidget;
class QGridLayout;
class QSplitter;
class FindDialog;
class AttributeWidget;
class TreeModel;
class TreeItem;
class TreeFilterProxyModel;

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
        Q_OBJECT
        
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
        
    private:
        Ui::MainWindow *ui;
        QTreeView *treeView;
        QSplitter *horSplitter;
        QScrollArea *scrollArea;
        QWidget *widget;
        QGridLayout *gridLayout;
        QTableWidget *tableWidget;

        TreeFilterProxyModel *filterModel;
		TreeModel *model;
        QModelIndex selectedIndex;

        FindDialog *find;
        AttributeWidget *attWidget;

        void readXml(const QString &_fileName);

	private slots:
        void newFile();
		void openFile();
		void closeFile();
		void saveFile();
		void showInfo(const QModelIndex &index);
		void fillTableView(const QString &textToFind, const QStringList &filters);
        void selectTreeItem();
        void showFindDialog();
        void setNewModel();
        void insertNode();
        void insertSubnode();
        void removeNode();
        void addAttribute();
        void saveNewData();
        void aboutThis();
        void aboutQt();
};

#endif // MAINWINDOW_H
