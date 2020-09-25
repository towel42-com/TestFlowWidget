#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QDialog>
#include <memory>
class CFlowWidgetItem;
class QStandardItemModel;
class QListWidgetItem;
namespace Ui{ class CMainWindow; }
class CMainWindow : public QDialog
{
    Q_OBJECT
public:
    CMainWindow( QWidget *parent = 0);
    ~CMainWindow();
public Q_SLOTS:
    void slotStatusItemSelected( QListWidgetItem* /*xListWidgetItem*/ );
    void slotFlowWidgetItemSelected( CFlowWidgetItem* xItem, bool xSelected );
private:
    void mLoadStatuses();

    void mLoadStatus( const std::tuple< int, QString, QIcon > & ii );

    void mDemoFlowWidget();
    void mLoadFromXML();
    void mDumpFlowWidget();
    void mCollapseWidgetType( const QModelIndex& index );
    QStandardItemModel * fModel;
    std::unique_ptr< Ui::CMainWindow > fImpl;
};

#endif 
