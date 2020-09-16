#include "MainWindow.h"
#include <QTreeWidget>
#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QIcon>
#include <QToolBox>
int main( int argc, char** argv )
{
    QApplication appl( argc, argv );
    Q_INIT_RESOURCE( application );
    CMainWindow dlg;
    return dlg.exec();

    //auto toolbox = new QToolBox;
    //auto tw = new QTreeWidget;
    //tw->header()->setHidden(true);
    //tw->setObjectName("TW1");
    //new QTreeWidgetItem(tw, QStringList() << "SubFlowItem11");
    //auto ti2 = new QTreeWidgetItem(tw, QStringList() << "SubFlowItem12");
    //new QTreeWidgetItem(ti2, QStringList() << "SubSubFlowItem12");
    //toolbox->addItem(tw, QIcon( ":/Entity.png" ), "Flow Item 1" );

    //tw = new QTreeWidget;
    //tw->setObjectName("TW2");
    //tw->header()->setHidden(true);
    //new QTreeWidgetItem(tw, QStringList() << "SubFlowItem21");
    //ti2 = new QTreeWidgetItem(tw, QStringList() << "SubFlowItem22");
    //new QTreeWidgetItem(ti2, QStringList() << "SubSubFlowItem22");
    //toolbox->addItem(tw, "Flow Item 2");

    //tw = new QTreeWidget;
    //tw->setObjectName("TW3");
    //tw->header()->setHidden(true);
    //new QTreeWidgetItem(tw, QStringList() << "SubFlowItem31");
    //new QTreeWidgetItem(tw, QStringList() << "SubFlowItem32");
    //toolbox->addItem(tw, "Flow Item 3");

    //toolbox->show();
}

