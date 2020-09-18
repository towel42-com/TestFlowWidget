#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include <QDialog>
#include <memory>
class CFlowWidgetItem;
namespace Ui{ class CMainWindow; }
class CMainWindow : public QDialog
{
    Q_OBJECT
public:
    CMainWindow( QWidget *parent = 0);
    ~CMainWindow();

    void mDumpFlowWidget();
private:
    std::unique_ptr< Ui::CMainWindow > fImpl;
};

#endif 
