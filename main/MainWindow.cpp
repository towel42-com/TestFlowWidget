#include "MainWindow.h"
#include "ui_MainWindow.h"  

#include <QInputDialog>
CMainWindow::CMainWindow( QWidget* parent )
    : QDialog( parent ),
    fImpl( new Ui::CMainWindow )
{
    fImpl->setupUi( this );

    int stateID = 0;
    {
        auto flowItem = fImpl->flowWidget->mAddTopLevelItem( stateID++, "FlowItem 1" );
        flowItem->mSetIcon( QIcon( ":/Entity.png" ) );
        auto icon = flowItem->mIcon();
        Q_ASSERT( !icon.isNull() );
        auto subFlowItem1 = new CFlowWidgetItem( stateID++, "SubFlowItem 1-1", flowItem );
        subFlowItem1->mSetIcon( QIcon( ":/Entity.png" ) );
        auto subFlowItem12 = new CFlowWidgetItem( stateID++, "SubFlowItem 1-1-2", QIcon( ":/Entity.png" ), subFlowItem1 );
        auto subFlowItem2 = new CFlowWidgetItem( stateID++, "SubFlowItem 1-2", flowItem );
    }

    {
        auto  flowItem = new CFlowWidgetItem( stateID++, "FlowItem 2", fImpl->flowWidget );
        auto subFlowItem1 = new CFlowWidgetItem( stateID++, "SubFlowItem 2-1", flowItem );
        auto subFlowItem2 = fImpl->flowWidget->mAddItem( stateID++, "SubFlowItem 2-2", flowItem );
        auto subFlowItem12 = new CFlowWidgetItem( stateID++, "SubFlowItem 2-2-1", QIcon( ":/Entity.png" ), subFlowItem2 );
    }

    {
        auto  flowItem = new CFlowWidgetItem( stateID++, "FlowItem 3", fImpl->flowWidget );
        auto subFlowItem1 = new CFlowWidgetItem( stateID++, "SubFlowItem 3-1", flowItem );
        auto subFlowItem2 = new CFlowWidgetItem( stateID++, "SubFlowItem 3-2", flowItem );
        auto subFlowItem22 = new CFlowWidgetItem( stateID++, "SubFlowItem 3-2-1", QIcon( ":/Entity.png" ), subFlowItem2 );
    }

    //fImpl->flowWidget->show();
    mDumpFlowWidget();

    fImpl->takeButton->setEnabled( false );
    fImpl->placeButton->setEnabled( false );
    fImpl->hideButton->setEnabled( false );
    fImpl->unhideButton->setEnabled( false );
    fImpl->disableButton->setEnabled( false );
    fImpl->enableButton->setEnabled( false );

    connect( fImpl->takenItems, &QListWidget::itemSelectionChanged,
             [this]()
    {
        fImpl->placeButton->setEnabled( (fImpl->takenItems->selectedItems().size() == 1) );
    } );

    connect( fImpl->hiddenItems, &QListWidget::itemSelectionChanged,
             [this]()
    {
        fImpl->unhideButton->setEnabled( fImpl->hiddenItems->selectedItems().size() == 1 );
    } );

    connect( fImpl->disabledItems, &QListWidget::itemSelectionChanged,
             [this]()
    {
        fImpl->enableButton->setEnabled( fImpl->disabledItems->selectedItems().size() == 1 );
    } );

    connect( fImpl->dumpFlowWidgetBtn, &QPushButton::clicked,
             [this]()
    {
        mDumpFlowWidget();
    } );

    connect( fImpl->setText, &QAbstractButton::clicked,
             [this]()
    {
        auto selected = fImpl->flowWidget->mSelectedItem();
        if ( !selected )
            return;
        auto txt = fImpl->selected->text();
        auto pos = txt.lastIndexOf( '.' );
        if ( pos != -1 )
            txt = txt.mid( pos + 1 );
        selected->mSetText( txt );
    } );
    connect( fImpl->setToolTip, &QAbstractButton::clicked,
             [this]()
    {
        auto selected = fImpl->flowWidget->mSelectedItem();
        if ( !selected )
            return;
        selected->mSetToolTip( fImpl->toolTip->text() );
    } );

    connect( fImpl->flowWidget, &CFlowWidget::sigFlowWidgetItemSelected,
             [this]( CFlowWidgetItem* xItem, bool xSelected )
    {
        fImpl->takeButton->setEnabled( xItem );
        fImpl->disableButton->setEnabled( xItem );
        fImpl->hideButton->setEnabled( xItem );

        fImpl->unhideButton->setEnabled( (fImpl->hiddenItems->selectedItems().size() == 1) && (fImpl->flowWidget->mSelectedItem() != nullptr) );
        fImpl->enableButton->setEnabled( (fImpl->disabledItems->selectedItems().size() == 1) && (fImpl->flowWidget->mSelectedItem() != nullptr) );
        fImpl->placeButton->setEnabled( (fImpl->takenItems->selectedItems().size() == 1) );

        if ( xItem )
        {
            QString lText;
            if ( xSelected )
            {
                lText = xItem->mFullText();
            }
            fImpl->selected->setText( lText );
            fImpl->toolTip->setText( xItem->mToolTip() );

            lText = xItem->mDump( true, false );
            fImpl->jsonData->setPlainText( lText );
        }
    } );

    connect( fImpl->flowWidget, &CFlowWidget::sigFlowWidgetItemDoubleClicked,
             [this]( CFlowWidgetItem* xItem )
    {
        QString lText;
        if ( xItem )
        {
            lText = xItem->mFullText();
        }
        fImpl->doubleClicked->setText( lText );
    } );

    connect( fImpl->flowWidget, &CFlowWidget::sigFlowWidgetItemHovered,
             [this]( CFlowWidgetItem* xItem )
    {
        QString lText;
        if ( xItem )
        {
            lText = xItem->mFullText();
        }
        fImpl->hovered->setText( lText );
    } );

    connect( fImpl->takeButton, &QAbstractButton::clicked,
             [this]()
    {
        auto xItem = fImpl->flowWidget->mSelectedItem();
        if ( !xItem )
            return;

        fImpl->flowWidget->mTakeItem( xItem );
        auto lwItem = new QListWidgetItem( xItem->mFullText() );
        QVariant lValue;
        lValue.setValue( xItem );
        lwItem->setData( Qt::UserRole + 1, lValue );
        fImpl->takenItems->addItem( lwItem );

        mDumpFlowWidget();
    } );
    connect( fImpl->placeButton, &QAbstractButton::clicked,
             [this]()
    {
        auto lSelectedTaken = fImpl->takenItems->selectedItems();
        if ( lSelectedTaken.count() != 1 )
            return;

        auto lListWidgetItem = lSelectedTaken.front();
        auto lTakenItem = lListWidgetItem->data( Qt::UserRole + 1 ).value< CFlowWidgetItem* >();
        if ( !lTakenItem )
            return;

        delete lListWidgetItem;

        auto lItemSelected = fImpl->flowWidget->mSelectedItem();
        auto lItemParent = lItemSelected ? lItemSelected->mParentItem() : nullptr;

        auto lItems = QStringList();
        if ( lItemSelected != nullptr )
        {
            lItems << tr( "Before Current Selection" ) << tr( "After Current Selection" );
            lItems << tr( "Append as Child of Current Selection" ) << tr( "Insert as First Child of Current Selection" );
        }
        lItems << tr( "Append Top Level Item" ) << tr( "Insert as First Top Level Item" );

        bool aOK = false;
        QString lPrefix;
        if ( lItemSelected )
            lPrefix = tr( "<li>Current Selection: %1</li>" ).arg( lItemSelected->mFullText() );
        if ( lItemParent )
            lPrefix += tr( "<li>Parent of Selection: %1</li>" ).arg( lItemParent->mFullText() );
        
        if ( !lPrefix.isEmpty() )
            lPrefix = "<ul>" + lPrefix + "</ul>";
        auto lLabel = lPrefix + tr( "Place Where:" );
        auto lSelected = QInputDialog::getItem( this, tr( "Place Where:" ), lLabel, lItems, 0, false, &aOK );
        if ( !aOK )
            return;

        if ( lSelected == tr( "Append Top Level Item" ) )
        {
            fImpl->flowWidget->mAddTopLevelItem( lTakenItem );
        }
        else if ( lSelected == tr( "Insert as First Top Level Item" ) )
        {
            fImpl->flowWidget->mInsertTopLevelItem( 0, lTakenItem );
        }
        else if ( lItemSelected )
        {
            if ( (lSelected == tr( "After Current Selection" )) || (lSelected == tr( "Before Current Selection" )) )
            {
                bool lBefore = lSelected.startsWith( tr( "Before" ) );
                if ( lItemParent )
                    lItemParent->mInsertChild( lItemSelected, lTakenItem, lBefore );
                else
                    fImpl->flowWidget->mInsertTopLevelItem( lItemSelected, lTakenItem, lBefore );
            }
            else if ( lSelected == tr( "Append as Child of Current Selection" ) )
            {
                lItemSelected->mAddChild( lItemSelected );
            }
            else if ( lSelected == tr( "Insert as First Child of Current Selection" ) )
            {
                lItemSelected->mInsertChild( 0, lItemSelected );
            }
        }
        mDumpFlowWidget();
    } );
    connect( fImpl->hideButton, &QAbstractButton::clicked,
             [this]()
    {
        auto xItem = fImpl->flowWidget->mSelectedItem();
        if ( !xItem )
            return;

        xItem->mSetHidden( true );
        auto lwItem = new QListWidgetItem( xItem->mFullText() );
        QVariant lValue;
        lValue.setValue( xItem );
        lwItem->setData( Qt::UserRole + 1, lValue );
        fImpl->hiddenItems->addItem( lwItem );

    } );
    connect( fImpl->unhideButton, &QAbstractButton::clicked,
             [this]()
    {
        auto selected = fImpl->hiddenItems->selectedItems();
        if ( selected.count() != 1 )
            return;

        auto selectedItem = selected.front()->data( Qt::UserRole + 1 ).value< CFlowWidgetItem* >();
        if ( !selectedItem )
            return;

        selectedItem->mSetHidden( false );
        delete selected.front();
    } );

    connect( fImpl->disableButton, &QAbstractButton::clicked,
             [this]()
    {
        auto xItem = fImpl->flowWidget->mSelectedItem();
        if ( !xItem )
            return;

        xItem->mSetDisabled( true );
        auto lwItem = new QListWidgetItem( xItem->mFullText() );
        QVariant lValue;
        lValue.setValue( xItem );
        lwItem->setData( Qt::UserRole + 1, lValue );
        fImpl->disabledItems->addItem( lwItem );

    } );
    connect( fImpl->enableButton, &QAbstractButton::clicked,
             [this]()
    {
        auto selected = fImpl->disabledItems->selectedItems();
        if ( selected.count() != 1 )
            return;

        auto selectedItem = selected.front()->data( Qt::UserRole + 1 ).value< CFlowWidgetItem* >();
        if ( !selectedItem )
            return;

        selectedItem->mSetEnabled( true );
        delete selected.front();
    } );
}

CMainWindow::~CMainWindow()
{}

void CMainWindow::mDumpFlowWidget()
{
    auto lText = fImpl->flowWidget->mDump( false );
    fImpl->jsonData->setPlainText( lText );
}