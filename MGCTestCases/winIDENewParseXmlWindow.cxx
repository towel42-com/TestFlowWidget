#include "winIDENewParseXmlWindow.h"
#ifdef NO
#include "zinWindowMgr.h"
#include "ui_winIDENewParseXmlWindow.h"
#include "zinUtil.h"
#include "winDirectivesEditor.h"


#include <iostream>
#include <QSettings>
#include <QDebug>
#include <QDesktopWidget>
#include "winIDEProjectManagerDesignInformation.h"
#include "winIDEProjectManagerProjectSettings.h"
#include "winIDEProjectManagerRunSettings.h"
#include "winLintProjectSummary.h"
#include "winLintCompileDesign.h"
#include "winLintRunDesign.h"
#include "winLintRunResultSummary.h"
#include "winLintMethodology.h"
#include "zinUtil.h"
#include <projectManager.hxx>
#include "zinWindowMgr.h"

using namespace zinMiscUtil;

static const QString vlibQSettingsTag = "Vlib";
static const QString vLogQSettingsTag = "Vlog";
static const QString libQSettingsTag = "lib_files";
static const QString sdcQSettingsTag = "sdc_files";
static const QString countQSettingsTag = "count";
static const QString cmdQSettingsTag = "cmd_";

//Flow Navigator Widget names 
static const QString DESIGN_INFO_WIDGET_NAME      = "winIDEProjectManagerDesignInformationWidget";
static const QString LINT_METHODOLOGY_WIDGET_NAME = "winLintMethodologyWidget";
static const QString PROJECT_SETTINGS_WIDGET_NAME = "winIDEProjectManagerProjectSettingsWidget";
static const QString RUN_SETTINGS_WIDGET_NAME     = "winIDEProjectManagerRunSettingsWidget";
static const QString LINT_PROJECT_SUMMARY_WIDGET_NAME = "winLintProjectSummaryWidget";
static const QString LINT_COMPILE_DESIGN_WIDGET_NAME  = "winLintCompileDesignWidget";
static const QString LINT_RUN_DESIGN_WIDGET_NAME      = "winLintRunDesignWidget";
//static const QString LINT_RUN_RESULT_SUMMARY_WIDGET_NAME  = "winLintRunResultSummaryWidget";

QString getDesignInformationWidgetName() { return DESIGN_INFO_WIDGET_NAME;}
QString getLintMethodologyWidgetName()   { return LINT_METHODOLOGY_WIDGET_NAME;}
QString getProjectSettingsWidgetName()   { return PROJECT_SETTINGS_WIDGET_NAME;}
QString getRunSettingsWidgetName()       { return RUN_SETTINGS_WIDGET_NAME;}
QString getLintProjectSummaryWidgetName(){ return LINT_PROJECT_SUMMARY_WIDGET_NAME;}
QString getLintCompileDesignWidgetName() { return LINT_COMPILE_DESIGN_WIDGET_NAME;}
QString getLintRunDesignWidgetName()     { return LINT_RUN_DESIGN_WIDGET_NAME;}
//QString getLintRunResultSummaryWidgetName()     { return LINT_RUN_RESULT_SUMMARY_WIDGET_NAME;}

static bool FLOW_NAVIGATOR_DEBUG = (osd_getenv("FLOW_NAVIGATOR_DEBUG") != NULL);

extern QString getLintFlowNavigatorCompileDesignStepString();
extern QString getLintFlowNavigatorRunLintStepString();
extern QString getCdcFlowNavigatorRunCdcAnalysisStepString();
extern QString getCdcCreateNetlistString();
extern QString getCdcRunClockAnalyisString();
extern QString getCdcDesignErrors();
extern QString getRdcFlowNavigatorRunRdcAnalysisStepString();
extern QString getRdcFlowNavigatorRunResetAnalysisStepString();
extern bool IDE_USE_PLASTIQUE_STYLE;

// This stylesheet is also used in Lint Msg summary view
QString getIDEFlowNavigatorStyleSheet()
{
    QString lBlueSelectionColor = "#cbdaf1";
    QString lBorderColor        = "#bfcde4";
    QString lBackgroundColor = "#f6f6f6";
    QString lForegroundColor = "#000000";
    QString lDisabledColor = "#696969";
    QString treeWidgetStylesheet ;
    // NS: with the background color set as f6f6f6 below, this alternate background doesn;t make sense - both gray shades.
    // QString("QTreeWidget { alternate-background-color: #f5f5f5; }");

    /*   treeWidgetStylesheet += QString("QTreeWidget::item:selected { \
border: 1px solid #567dbc; \
}");*/


    /* Below is commented as we are using QTreeWidget::item:selected:active
       treeWidgetStylesheet += QString("QTreeWidget::item:selected:active{ \
background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6ea1f1, stop: 1 #567dbc); \
font: bold 100px; \
}"); 
*/

    treeWidgetStylesheet += QString("QTreeWidget::item:selected:active{ \
            background: %1; \
            border: 1px solid %2; \
            }").arg(lBlueSelectionColor).arg(lBorderColor);

/* When item is clicked gray color is seen so below is added to hide the gray color*/
treeWidgetStylesheet += QString("QTreeWidget::item:selected:!active{ \
        background: %1; \
        border: 1px solid %2; \
        }").arg(lBlueSelectionColor).arg(lBorderColor);

/* Dark Blue Color is seen around branches so as to hide that complete branch color is set as white */
treeWidgetStylesheet += QString("QTreeWidget::branch{\
        background-color: %1; \
        }").arg(lBackgroundColor);


//NS: We do not have vline.png... Not sure why we added.. Commenting it....
/*
treeWidgetStylesheet += QString("QTreeWidget::branch:has-siblings:!adjoins-item { \
        border-image: url(%1) 0; \
        }").arg(zinImage::getPath("vline.png"));
*/

treeWidgetStylesheet += QString("QTreeWidget::branch:has-siblings:adjoins-item { \
        border-image: url(%1) 0; \
        }").arg(zinImage::getPath("branch-more.png"));

treeWidgetStylesheet += QString("QTreeWidget::branch:!has-children:!has-siblings:adjoins-item { \
        border-image: url(%1) 0; \
        }").arg(zinImage::getPath("branch-end.png"));

treeWidgetStylesheet += QString("QTreeWidget::branch:has-children:!has-siblings:closed, \
        QTreeWidget::branch:closed:has-children:has-siblings { \
        border-image: none; \
        image: url(%1); \
        }").arg((IDE_USE_PLASTIQUE_STYLE) ? zinImage::getPath("plastique_branch_closed.png") : zinImage::getPath("branch-closed.png"));


treeWidgetStylesheet += QString("QTreeView::branch:open:has-children:!has-siblings, \
        QTreeWidget::branch:open:has-children:has-siblings  { \
        border-image: none; \
        image: url(%1); \
        }").arg((IDE_USE_PLASTIQUE_STYLE) ? zinImage::getPath("plastique_branch_open.png") : zinImage::getPath("branch-open.png"));

treeWidgetStylesheet += QString("QTreeWidget {background-color: %1}").arg(lBackgroundColor);
treeWidgetStylesheet += QString("QTreeWidget {color: %1}").arg(lForegroundColor);
treeWidgetStylesheet += QString("QTreeWidget::item:selected {color: %1}").arg(lForegroundColor);
/* Since the width of Second Column of tree Widget is 4px so margin-top is set as 4px */
treeWidgetStylesheet += QString("QTreeWidget {padding: 3px;} QTreeWidget::item { margin-top: 4px; }");

treeWidgetStylesheet += QString("QTreeView::item:disabled { \
        color: %1; \
        }").arg(lDisabledColor);

//qDebug() << "treeWidgetStylesheet : " << treeWidgetStylesheet;
//treeWidgetStylesheet += QString("QTreeWidget::item{padding: 5px;}");
return treeWidgetStylesheet;
}

winIDENewParseXmlWindow::winIDENewParseXmlWindow(QWidget *parent) : QWidget(parent),
                                                                    winIDENewTreeWidgetMainWindowUI(new Ui::winIDENewTreeWidgetMainWindow)
                                                                    , _dIDEXmlDomParser(NULL)
                                                                    , winIDEParseTclCommandUIInstance(NULL)
                                                                    , _parentWidget(parent)
{
    QString winIDENewXmlPath;
    QString rtldGui = osd_getenv("RTLD_GUI");
    if (!rtldGui.isEmpty()) {
        if (!rtldGui.endsWith(IDE_DIR_SEP))
            rtldGui += IDE_DIR_SEP;
        ProjectManager* lPrjMgr = ProjectManager::Instance();
        if (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_LINT) {
            winIDENewXmlPath = rtldGui + QString("..") + QString(IDE_DIR_SEP) + "idegui" + QString(IDE_DIR_SEP) + "xml" + QString(IDE_DIR_SEP) + "LintProjectManagerFlow.xml";
        } else if (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC){
            winIDENewXmlPath = rtldGui + QString("..") + QString(IDE_DIR_SEP) + "idegui" + QString(IDE_DIR_SEP) + "xml" + QString(IDE_DIR_SEP) + "CDCProjectManagerFlow.xml";
        } else if (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC){
            winIDENewXmlPath = rtldGui + QString("..") + QString(IDE_DIR_SEP) + "idegui" + QString(IDE_DIR_SEP) + "xml" + QString(IDE_DIR_SEP) + "RDCProjectManagerFlow.xml";
        } 
        else
        {
            QString message = "Got an invalid tool type from the ProjectManager";
            QWidget* mainwindow = (QWidget*)VXCMGR->mGetMainWindow();
            int ret = QMessageBox::critical(mainwindow, tr("Project Manager"),
                    message, QMessageBox::Ok);

            if(FLOW_NAVIGATOR_DEBUG)
            {
                qDebug() << "Got an invalid tool from project manager inside winIDENewParseXmlWindow";
            }
            if (ret == QMessageBox::Ok)
            {
                return;
            }
        }
    }
     winIDENewTreeWidgetMainWindowUI->setupUi(this);
    _dIDEXmlDomParser = new winIDENewXmlDomParser(winIDENewTreeWidgetMainWindowUI);

    connect(winIDENewTreeWidgetMainWindowUI->treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(slotTreeWidgetItemClicked(QTreeWidgetItem*, int)));

    _dIDEXmlDomParser->readFile(winIDENewXmlPath);

    winIDENewTreeWidgetMainWindowUI->treeWidget->setStyleSheet(getIDEFlowNavigatorStyleSheet());

    winIDENewTreeWidgetMainWindowUI->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    winIDENewTreeWidgetMainWindowUI->treeWidget->setSelectionBehavior(QAbstractItemView::SelectRows);

    winIDENewParentWidgetForFlowNavigatorWidgetsInst = new winIDENewParentWidgetForFlowNavigatorWidgets(parent);

    populateMapsAndAddWidgetsToStackWidget();

    QTreeWidget* tree =  winIDENewTreeWidgetMainWindowUI->treeWidget;
    tree->expandAll();   
    
    connect(tree,SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)),this, SLOT(currentItemChangedSlot(QTreeWidgetItem *,QTreeWidgetItem*)));

    stepToStepDataMap = _dIDEXmlDomParser->getStepToStepDataMap();
    
    winIDENewTreeWidgetMainWindowUI->treeWidget->setEnabled(false);  

    winIDENewTreeWidgetMainWindowUI->treeWidget->setColumnCount(2);
    winIDENewTreeWidgetMainWindowUI->treeWidget->setColumnWidth(1,4);
    winIDENewTreeWidgetMainWindowUI->treeWidget->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    winIDENewTreeWidgetMainWindowUI->treeWidget->header()->hide();

    //CDC MODE - If CDC mode, hide the second column
    //if ( (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC) ||
    //     (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC)
    //   )
    //{
    //   winIDENewTreeWidgetMainWindowUI->treeWidget->setColumnHidden(1, true); 
    //}
}

winIDENewParseXmlWindow::~winIDENewParseXmlWindow()
{
    if(winIDENewTreeWidgetMainWindowUI)
    {
        delete winIDENewTreeWidgetMainWindowUI;
        winIDENewTreeWidgetMainWindowUI = NULL;
    }

    if(_dIDEXmlDomParser)
    {
        delete _dIDEXmlDomParser;
        _dIDEXmlDomParser = NULL;
    }

    if (winIDENewParentWidgetForFlowNavigatorWidgetsInst)
    {
        delete winIDENewParentWidgetForFlowNavigatorWidgetsInst;
        winIDENewParentWidgetForFlowNavigatorWidgetsInst = NULL;
    }
}


QTreeWidget* winIDENewParseXmlWindow::getTreeWidget() 
{
    return winIDENewTreeWidgetMainWindowUI->treeWidget; 
}

void winIDENewParseXmlWindow::populateMapsAndAddWidgetsToStackWidget()
{
    ProjectManager* lPrjMgr = ProjectManager::Instance();
    if ( (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC)  ||
          (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC)
       )
    {
        QWidget* winIDEProjectManagerDesignInformationDialogInst = new winIDEProjectManagerDesignInformationWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getDesignInformationWidgetName()] = winIDEProjectManagerDesignInformationDialogInst;
        widgetToClassNameMap[winIDEProjectManagerDesignInformationDialogInst] = getDesignInformationWidgetName();

        QWidget* winIDEProjectManagerProjectSettingsDialogInst = new winIDEProjectManagerProjectSettingsWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getProjectSettingsWidgetName()] = winIDEProjectManagerProjectSettingsDialogInst;
        widgetToClassNameMap[winIDEProjectManagerProjectSettingsDialogInst] = getProjectSettingsWidgetName();

        QWidget* winIDEProjectManagerRunSettingsDialogInst = new winIDEProjectManagerRunSettingsWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getRunSettingsWidgetName()] = winIDEProjectManagerRunSettingsDialogInst;
        widgetToClassNameMap[winIDEProjectManagerRunSettingsDialogInst] = getRunSettingsWidgetName();

        /* We want to maintain the order of insertion in QStackWidget. But if we iterate over QMap it will give sorted order and then insert.
         * That is why We can't use the following way for insertion.
         * for(auto classNameToClassInstanceMapIt = classNameToClassInstanceMap.begin(); classNameToClassInstanceMapIt!=classNameToClassInstanceMap.end() ;++classNameToClassInstanceMapIt)
         {
         QWidget* widgetInstance = classNameToClassInstanceMapIt.value();
         if( widgetInstance )
         {
         winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(widgetInstance);
         }
         }


IMPORTANT : It is absolutely essential to ensure that the order of adding widgets to stack widget is in sync with
tree widget items of the flow navigator.
*/
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerDesignInformationDialogInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerProjectSettingsDialogInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerRunSettingsDialogInst);
    } 
    else if (lPrjMgr->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_LINT)
    {

        QWidget* winIDEProjectManagerDesignInformationDialogInst = new winIDEProjectManagerDesignInformationWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getDesignInformationWidgetName()] = winIDEProjectManagerDesignInformationDialogInst;
        widgetToClassNameMap[winIDEProjectManagerDesignInformationDialogInst] = getDesignInformationWidgetName();

        QWidget* winLintMethodologyWidgetInst = new winLintMethodologyWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getLintMethodologyWidgetName()] = winLintMethodologyWidgetInst;
        widgetToClassNameMap[winLintMethodologyWidgetInst] = getLintMethodologyWidgetName();
        
        QWidget* winIDEProjectManagerProjectSettingsDialogInst = new winIDEProjectManagerProjectSettingsWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getProjectSettingsWidgetName()] = winIDEProjectManagerProjectSettingsDialogInst;
        widgetToClassNameMap[winIDEProjectManagerProjectSettingsDialogInst] = getProjectSettingsWidgetName();

        QWidget* winIDEProjectManagerRunSettingsDialogInst = new winIDEProjectManagerRunSettingsWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getRunSettingsWidgetName()] = winIDEProjectManagerRunSettingsDialogInst;
        widgetToClassNameMap[winIDEProjectManagerRunSettingsDialogInst] = getRunSettingsWidgetName();
        
        QWidget* winLintProjectSummaryWidgetInst = new winLintProjectSummaryWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getLintProjectSummaryWidgetName()] = winLintProjectSummaryWidgetInst;
        widgetToClassNameMap[winLintProjectSummaryWidgetInst] = getLintProjectSummaryWidgetName();

        QWidget* winLintCompileDesignWidgetInst = new winLintCompileDesignWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getLintCompileDesignWidgetName()] = winLintCompileDesignWidgetInst;
        widgetToClassNameMap[winLintCompileDesignWidgetInst] = getLintCompileDesignWidgetName();

        QWidget* winLintRunDesignWidgetInst = new winLintRunDesignWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getLintRunDesignWidgetName()] = winLintRunDesignWidgetInst;
        widgetToClassNameMap[winLintRunDesignWidgetInst] = getLintRunDesignWidgetName();
        
        /*
        QWidget* winLintRunResultSummaryWidgetInst = new winLintRunResultSummaryWidget(winIDENewParentWidgetForFlowNavigatorWidgetsInst);
        classNameToClassInstanceMap[getLintRunResultSummaryWidgetName()] = winLintRunResultSummaryWidgetInst;
        widgetToClassNameMap[winLintRunResultSummaryWidgetInst] = getLintRunResultSummaryWidgetName();*/


        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerDesignInformationDialogInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winLintMethodologyWidgetInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerProjectSettingsDialogInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winIDEProjectManagerRunSettingsDialogInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winLintProjectSummaryWidgetInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winLintCompileDesignWidgetInst);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winLintRunDesignWidgetInst);
        //winIDENewParentWidgetForFlowNavigatorWidgetsInst->addWidgetToStackWidget(winLintRunResultSummaryWidgetInst);
    }
}


void winIDENewParseXmlWindow::highlightItemForWidget(QWidget* curWidget)
{
    if ( (curWidget != nullptr) && widgetToClassNameMap.contains(curWidget) )
    {
        QString className = widgetToClassNameMap[curWidget];
        QTreeWidgetItemIterator it(winIDENewTreeWidgetMainWindowUI->treeWidget);
        while (*it)
        {
            QTreeWidgetItem* item = *it;
            QVariant savedStepId = item->data(0, XML_PARSER_ROLE);
            QString stepId;
            if( savedStepId.canConvert<QString>())
            {
                stepId = savedStepId.value<QString>();
                QString uiClassFromParsedXml = stepToStepDataMap[stepId]->getUiClass();
                if( className == uiClassFromParsedXml)
                {
                    winIDENewTreeWidgetMainWindowUI->treeWidget->setCurrentItem(item);
                    item->setSelected(true);
                    break;
                }
            }
            ++it;
        }
    }
}

void winIDENewParseXmlWindow::currentItemChangedSlot(QTreeWidgetItem* currentItem ,QTreeWidgetItem* previousItem)
{
    if (!currentItem)
    {
        return;
    }
    QVariant savedStepId = currentItem->data(0, XML_PARSER_ROLE);
    QString stepId;
    if( savedStepId.canConvert<QString>() && winIDENewParentWidgetForFlowNavigatorWidgetsInst )
    {
        stepId = savedStepId.value<QString>();
        QString iconName = stepToStepDataMap[stepId]->getIcon();
        QIcon icon = zinImage::get(iconName);
        winIDENewParentWidgetForFlowNavigatorWidgetsInst->setIconForParentFlowNavWidget(icon);
    }
}

void winIDENewParseXmlWindow::slotTreeWidgetItemClicked(QTreeWidgetItem* item , int columnIndex)
{
    if (item->isDisabled())
        return;
    //VISU-18166 - Lint Qt GUI - unable to modify Flow Navigator if lint.db is loaded  
    if (ProjectManager::Instance()->GetPrjFile().empty())
    {
        //Pop the Save As Dialog
        MainWindow::getMainWindowHandle()->saveAsProjectFileSlot();
        //VISU-20402 fixed (Redundant actions to open flow navigator views)
        //return;
        if (ProjectManager::Instance()->GetPrjFile().empty())
            return;
    }

    QVariant savedStepId = item->data(0, XML_PARSER_ROLE);
    QString stepId;
    if( savedStepId.canConvert<QString>())
    {
        bool lOpenView = false;
        stepId = savedStepId.value<QString>();

        QString tclProcFromParsedXml = stepToStepDataMap[stepId]->getTclProc();
        QString stepNameFromParsedXml = stepToStepDataMap[stepId]->getName();
        QString uiClassFromParsedXml = stepToStepDataMap[stepId]->getUiClass();
        QString iconToShow           = stepToStepDataMap[stepId]->getIcon();

        QWidget *widgetToShow = classNameToClassInstanceMap[uiClassFromParsedXml];
        if ( widgetToShow )
        {
            lOpenView = true;
            winIDENewParentWidgetForFlowNavigatorWidgetsInst->setCurrentWidgetInStackedWidget(widgetToShow);
            //Sumita - refreshing it so that there is no lag
            QApplication::processEvents();
            update();
        }
        if ( winIDENewParentWidgetForFlowNavigatorWidgetsInst->isVisible() == false && widgetToShow)
        {
            QDesktopWidget* desktop = QApplication::desktop();

            int screenWidth = desktop->width();
            int screenHeight = desktop->height();

            winIDENewParentWidgetForFlowNavigatorWidgetsInst->setMinimumWidth(screenWidth - screenWidth/2);
            winIDENewParentWidgetForFlowNavigatorWidgetsInst->setMinimumHeight(screenHeight - screenHeight/1.3);
            winIDENewParentWidgetForFlowNavigatorWidgetsInst->show();
        }
        else if(stepNameFromParsedXml == getCdcFlowNavigatorRunCdcAnalysisStepString())
        {
            //Sumita: If the design is running, return
            if (item->flags() == Qt::NoItemFlags)
                return;
            int ret = QMessageBox::question(this, tr("CDC Design Run"),
                    tr("Do you want to run the design?"), 
                    tr("&Yes"), tr("&No"), 0, 0, 1);
            if (ret == true) {  
                return; //user cancelled
            } else {
                if (FLOW_NAVIGATOR_DEBUG) {
                    ProjectManager* prjMgr = ProjectManager::Instance() ;
                    string od = prjMgr->GetOutputDirectory();
                    qDebug() << "During Run Design : Output directory set is "<<QString::fromStdString(od);
                }
                QString res;
                //Setting the flag to "NoItemFlags" when the design is run
                Qt::ItemFlags flags =  item->flags();
                item->setFlags(Qt::NoItemFlags);
                bool retVal = false;
                retVal = VXCMGR->mExecuteTCL("__cdc_run", res);
                //Setting the item to the defaut flag when the run is complete
                item->setFlags(flags);
            }
        }
        else if (stepNameFromParsedXml == getRdcFlowNavigatorRunRdcAnalysisStepString())
        {
            //Sumita: If the design is running, return
            if (item->flags() == Qt::NoItemFlags)
                return;
            int ret = QMessageBox::question(this, tr("RDC Design Run"),
                    tr("Do you want to run the design?"), 
                    tr("&Yes"), tr("&No"), 0, 0, 1);
            if (ret == true) {  
                return; //user cancelled
            } else {
                if (FLOW_NAVIGATOR_DEBUG) {
                    ProjectManager* prjMgr = ProjectManager::Instance() ;
                    string od = prjMgr->GetOutputDirectory();
                    qDebug() << "During Run Design : Output directory set is "<<QString::fromStdString(od);
                }
                QString res;
                //Setting the flag to "NoItemFlags" when the design is run
                Qt::ItemFlags flags =  item->flags();
                item->setFlags(Qt::NoItemFlags);
                bool retVal = false;
                retVal = VXCMGR->mExecuteTCL("__rdc_run", res);
                //Setting the item to the defaut flag when the run is complete
                item->setFlags(flags);
            }
        }
        else if (stepNameFromParsedXml == getLintFlowNavigatorCompileDesignStepString())
        {
            if (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC || 
                ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC)
            {
                if (item->flags() == Qt::NoItemFlags)
                    return;
                int ret = QMessageBox::question(this, tr("Compile Design"),
                        tr("Do you want to compile the design?"), 
                        tr("&Yes"), tr("&No"), 0, 0, 1);
                if (ret == true) {  
                    return; //user cancelled
                } else {
                    if (FLOW_NAVIGATOR_DEBUG) 
                    {
                        ProjectManager* prjMgr = ProjectManager::Instance() ;
                        string od = prjMgr->GetOutputDirectory();
                        qDebug() << "During Compile Design: Output directory set is "<<QString::fromStdString(od);
                    }
                    QString res;
                    //Setting the flag to "NoItemFlags" when the design is run
                    Qt::ItemFlags flags =  item->flags();
                    item->setFlags(Qt::NoItemFlags);
                    bool retVal = false;
                    retVal = VXCMGR->mExecuteTCL("__compile_design", res);
                    //Setting the item to the defaut flag when the run is complete
                    item->setFlags(flags);
                }
            }
        }
        else if (stepNameFromParsedXml == getCdcCreateNetlistString()) 
        {
            if (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC || 
                ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC)
            {
                if (item->flags() == Qt::NoItemFlags)
                    return;
                int ret = QMessageBox::question(this, tr("Create Netlist"),
                        tr("Do you want to create the netlist?"), 
                        tr("&Yes"), tr("&No"), 0, 0, 1);
                if (ret == true) {  
                    return; //user cancelled
                } else {
                    if (FLOW_NAVIGATOR_DEBUG) 
                    {
                        ProjectManager* prjMgr = ProjectManager::Instance() ;
                        string od = prjMgr->GetOutputDirectory();
                        qDebug() << "During Create Netlist: Output directory set is "<<QString::fromStdString(od);
                    }
                    QString res;
                    //Setting the flag to "NoItemFlags" when the design is run
                    Qt::ItemFlags flags =  item->flags();
                    item->setFlags(Qt::NoItemFlags);
                    bool retVal = false;
                    retVal = VXCMGR->mExecuteTCL("__create_netlist", res);
                    //Setting the item to the defaut flag when the run is complete
                    item->setFlags(flags);
                }
            }
        }
        else if (stepNameFromParsedXml == getCdcRunClockAnalyisString())
        {
            if (item->flags() == Qt::NoItemFlags)
                return;
            int ret = QMessageBox::question(this, tr("CDC Clock Analysis"),
                    tr("Do you want to run Clock Analyis?"), 
                    tr("&Yes"), tr("&No"), 0, 0, 1);
            if (ret == true) {  
                return; //user cancelled
            } else {
                if (FLOW_NAVIGATOR_DEBUG) {
                    ProjectManager* prjMgr = ProjectManager::Instance() ;
                    string od = prjMgr->GetOutputDirectory();
                    qDebug() << "During Clock Analysis : Output directory set to "<<QString::fromStdString(od);
                }
                QString res;
                //Setting the flag to "NoItemFlags" when the design is run
                Qt::ItemFlags flags =  item->flags();
                item->setFlags(Qt::NoItemFlags);
                bool retVal = false;
                retVal = VXCMGR->mExecuteTCL("__clock_analysis", res);
                //Setting the item to the defaut flag when the run is complete
                item->setFlags(flags);
            }
        }
        else if (stepNameFromParsedXml == getRdcFlowNavigatorRunResetAnalysisStepString())
        {
            if (item->flags() == Qt::NoItemFlags)
                return;
            int ret = QMessageBox::question(this, tr("RDC Reset Analysis"),
                    tr("Do you want to run Reset Analyis?"), 
                    tr("&Yes"), tr("&No"), 0, 0, 1);
            if (ret == true) {  
                return; //user cancelled
            } else {
                if (FLOW_NAVIGATOR_DEBUG) {
                    ProjectManager* prjMgr = ProjectManager::Instance() ;
                    string od = prjMgr->GetOutputDirectory();
                    qDebug() << "During Reset Analysis : Output directory set to "<<QString::fromStdString(od);
                }
                QString res;
                //Setting the flag to "NoItemFlags" when the design is run
                Qt::ItemFlags flags =  item->flags();
                item->setFlags(Qt::NoItemFlags);
                VXCMGR->mExecuteTCL("__reset_analysis", res);
                //Setting the item to the defaut flag when the run is complete
                item->setFlags(flags);
            }
        }
        else if (stepNameFromParsedXml == getCdcDesignErrors())
        {
            if (WMGR->mWindowIsOpen(WID_MSG_VIEWER)) {

                WMGR->mViewWindow(WID_MSG_VIEWER);
            }
            ProjectManager::Instance()->SetIsDirty(true);
            WMGR->mGetCdcWindowState()->mGetCdcStateMachineMgr()->mSetStatusForPrjMgrAndStages(
                            ProjectCommandStageE::PROJECT_COMMAND_STAGE_DESIGN_ERRORS, 
                            ProjectCommandStatusE::PROJECT_COMMAND_STATUS_EXECUTED_SUCCESS);

        }
    }
}

QWidget* winIDENewParseXmlWindow::getWidget(QString name) {
  return NULL;
}

void winIDENewParseXmlWindow::run() {
}


void winIDENewParseXmlWindow::setEnableDesignErrors(bool flag)
{
    if( (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_CDC) ||
        (ProjectManager::Instance()->GetToolType() == zi_::ProjectCommandToolTypeE::PROJECT_COMMAND_TOOL_RDC)
      )
    {
        const QString findText1 = "Design Errors/Warnings";
        QList<QTreeWidgetItem *> item1List = winIDENewTreeWidgetMainWindowUI->treeWidget->findItems(findText1, Qt::MatchExactly | Qt::MatchRecursive); 
        if (item1List.size() > 0)
        {
            QTreeWidgetItem* item1 = item1List[0];
            if (item1) 
            {
                //qDebug() <<"setEnableDesignErrors called";
                item1->setDisabled(!flag);
            }
        }
    }
}

void winIDENewParseXmlWindow::setEnableFlowNavigatorTreeWidget(bool flag, QString& text)
{
    if (text.endsWith(".prj"))
    {
        text.remove(".prj");
    }
    winIDENewTreeWidgetMainWindowUI->treeWidget->setEnabled(flag);
    QTreeWidgetItem* topLevelItem = winIDENewTreeWidgetMainWindowUI->treeWidget->topLevelItem(0);
    if (topLevelItem) 
    {
        topLevelItem->setText(0, text);
    }
}

void winIDENewParseXmlWindow::mShowNextStepWidget(QString stageName)
{
    QList<QTreeWidgetItem *> itemList = winIDENewTreeWidgetMainWindowUI->treeWidget->findItems(stageName, Qt::MatchExactly | Qt::MatchRecursive); 
    QTreeWidgetItem* item = itemList[0];
    if (item)
    {
        winIDENewTreeWidgetMainWindowUI->treeWidget->clearSelection();
        slotTreeWidgetItemClicked(item, 0);
        item->setSelected(true);
    }
}

void winIDENewParseXmlWindow::mSetDisableTreeWidgetItem(QString stageName, bool flag)
{
    QList<QTreeWidgetItem *> itemList = winIDENewTreeWidgetMainWindowUI->treeWidget->findItems(stageName, Qt::MatchExactly | Qt::MatchRecursive); 
    QTreeWidgetItem* item = itemList[0];
    if (!item)
        return;
    item->setDisabled(flag);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

winIDENewXmlDomParser::winIDENewXmlDomParser(Ui::winIDENewTreeWidgetMainWindow* winIDENewTreeWidgetMainWindowUI)
{
    treeWidget = winIDENewTreeWidgetMainWindowUI->treeWidget;
    m_winIDENewTreeWidgetMainWindowUI = winIDENewTreeWidgetMainWindowUI;
}

bool winIDENewXmlDomParser::readFile(const QString &fileName)
{
    if(fileName.isEmpty())
    {
        if(FLOW_NAVIGATOR_DEBUG)
        {
            qDebug() << "Error: winIDENewXmlDomParser::readFile: Empty file";
        }
        return false;
    }
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        if(FLOW_NAVIGATOR_DEBUG)
        {
            qDebug() << "Error: winIDENewXmlDomParser::readFile: Cannot read file" << fileName ;
            print_stack();
        }
        return false;
    }

    QString errorStr;
    int errorLine;
    int errorColumn;

    QDomDocument doc;
    if (!doc.setContent(&file, false, &errorStr, &errorLine,
                        &errorColumn)) {
        qDebug() << "Error: Parse error at line " << errorLine << ", "
                 << "column " << errorColumn << ": ";
        return false;
    }

    QDomElement root = doc.documentElement();
    xmlRootElement = root.tagName();
    {
        QTreeWidgetItem* header = treeWidget->headerItem();
        header->setText(0, xmlRootElement);
        QFont serifFont("Sans", 9, QFont::Bold);
        header->setFont(0, serifFont);

        QDomNode stepsNode = root.firstChild();
        QDomElement stepsElement = stepsNode.toElement();
        QString stepsTag = stepsElement.tagName();

        parseStepsElement(stepsElement, header);
    }

    return true;
}

void winIDENewXmlDomParser::parseStepsElement(const QDomElement& element, QTreeWidgetItem* parent)
{
    QDomNode firstChildOfElementPassed = element.firstChild();

    while (!firstChildOfElementPassed.isNull())
    {
        QDomElement xmlNodeElement = firstChildOfElementPassed.toElement();
        parseStepsElementRecur(xmlNodeElement,parent);
        firstChildOfElementPassed = firstChildOfElementPassed.nextSibling();
    }
}

void winIDENewXmlDomParser::parseStepsElementRecur(const QDomElement& element, QTreeWidgetItem* parent)
{
    QDomNode firstChildOFXmlNode = element.firstChild();
    QString name;
    QString icon;
    QString tclProc;
    QString status;
    QString id;
    QString uiClass;
    winIDENewParsedXmlStepData* parsedXmlStepData;
    QTreeWidgetItem* itemInTree = NULL;

    while(!firstChildOFXmlNode.isNull())
    {
        QDomElement firstChildOFXmlNodeElement = firstChildOFXmlNode.toElement();
        QString tagName = firstChildOFXmlNodeElement.tagName();
        QString valueInsideTag = firstChildOFXmlNodeElement.text();
        if(tagName == "id")
        {
            id = valueInsideTag;
            if( parent->text(0) == xmlRootElement) // Header Element
            {

                itemInTree = new QTreeWidgetItem(treeWidget);
                parent->addChild(itemInTree);
            }
            else
            {
                itemInTree = new QTreeWidgetItem(parent);
                
                //set a vertical line for second column in tree widget for status
                QWidget *verticalLineWidget = new QWidget;
                verticalLineWidget->setFixedWidth(4);
                verticalLineWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                verticalLineWidget->setStyleSheet(QString("background-color: #c0c0c0;"));
                treeWidget->setItemWidget(itemInTree, 1, verticalLineWidget);
            }

            QVariant stepId;
            stepId.setValue(id);
            itemInTree->setData(0, XML_PARSER_ROLE, stepId );
        }
        else if(tagName == "name")
        {
            itemInTree->setText(0, valueInsideTag);
            name = valueInsideTag;
        }
        else if(tagName == "ui")
        {
            uiClass = valueInsideTag;
        }
        else if(tagName == "Step")
        {
            parseStepsElementRecur(firstChildOFXmlNodeElement,itemInTree);
        }
        else if(tagName == "icon")
        {
            icon = valueInsideTag;
            itemInTree->setIcon(0, zinImage::get(icon) );
        }
        
        parsedXmlStepData = new winIDENewParsedXmlStepData(name, icon, tclProc, status, uiClass);
        stepToStepDataMap[id] = parsedXmlStepData;

        firstChildOFXmlNode = firstChildOFXmlNode.nextSibling();
    }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

#endif