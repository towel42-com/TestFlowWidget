#include "MainWindow.h"
#include "SABUtils/FileUtils.h"
#include <QApplication>
#include <QDebug>
#include <cstring>
#include <iostream>

#include <QTreeWidget>

int main( int argc, char** argv )
{
    // std::cout << sizeof( uintptr_t ) << "\n";

    QApplication appl( argc, argv );
    Q_INIT_RESOURCE( application );

    QString lFileName;
    for( int ii = 1; ii < argc; ++ii )
    {
        if ( std::strncmp( argv[ ii ], "-xml", 4 ) == 0 )
        {
            if ( ++ii < argc )
                lFileName = argv[ ii ];
        }
    }

    CMainWindow dlg;
    if ( !lFileName.isEmpty() )
        dlg.mLoadFromXML( lFileName );
    return dlg.exec();
}

