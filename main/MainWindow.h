#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QDialog>
#include <memory>
class CFlowWidgetItem;
class QStandardItemModel;
namespace Ui{ class CMainWindow; }
class CMainWindow : public QDialog
{
    Q_OBJECT
public:
    CMainWindow( QWidget *parent = 0);
    ~CMainWindow();
private:
    void mResetFlowWidget();
    void mDumpFlowWidget();
    void mCollapseWidgetType( const QModelIndex& index );
    QStandardItemModel * fModel;
    std::unique_ptr< Ui::CMainWindow > fImpl;
};

#endif 
