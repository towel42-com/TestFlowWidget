#undef QT_NO_DEBUG_OUTPUT
#include "MainWindow.h"
#include "SABUtils/FileUtils.h"
#include <QApplication>
#include <QDebug>

int main( int argc, char** argv )
{
    QApplication appl( argc, argv );
    Q_INIT_RESOURCE( application );

    CMainWindow dlg;
    return dlg.exec();
}

