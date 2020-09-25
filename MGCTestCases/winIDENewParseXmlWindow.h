#ifdef NO
#ifndef WINIDENEWPARSEXMLWINDOW
#define WINIDENEWPARSEXMLWINDOW

#include <QTreeWidget>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QDomElement>
#include <QSplitter>
#include <QMap>
#include <QWizard>
#include "winIDEParseTclCommandUI.h"
#include "ui_winIDENewParentWidgetForFlowNavigatorWidgets.h"
#include "winIDENewParentWidgetForFlowNavigatorWidgets.h"

static const int XML_PARSER_ROLE = Qt::UserRole + 1;
static const QString DEFAULT_TEXT_EDIT_STRING = "Click on the steps below to execute procedure .";

namespace Ui
{
  class winIDENewTreeWidgetMainWindow;
  class winIDENewParentWidgetForFlowNavigatorWidgets;
}

class winIDENewXmlDomParser;
class winIDENewParsedXmlStepData;
class winIDENewParentWidgetForFlowNavigatorWidgets;

////////////////////////////////////////// command objects ///////////////////////////////////////


////////////////////////////////////////// command objects ///////////////////////////////////////
class winIDENewParseXmlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit winIDENewParseXmlWindow(QWidget *parent = 0);
    ~winIDENewParseXmlWindow();
    void highlightItemForWidget(QWidget* curWidget);
    
    winIDENewParentWidgetForFlowNavigatorWidgets* getParentWidgetForFlowNavigator() {return winIDENewParentWidgetForFlowNavigatorWidgetsInst;}
    QWidget* getPageWithName(const QString& widgetName)
    {
        QWidget* widget = classNameToClassInstanceMap[widgetName];
        return widget;
    }
    QString getPageNameFromWidget(QWidget* widget)
    {
      QString pageName = widgetToClassNameMap[widget];
      return pageName;
    }
    void populateMapsAndAddWidgetsToStackWidget();
    QTreeWidget* getTreeWidget();
    void setEnableFlowNavigatorTreeWidget(bool, QString&);
    void setEnableDesignErrors(bool);
    void mShowNextStepWidget(QString stageName);
    void mSetDisableTreeWidgetItem(QString stageName, bool flag);

public slots:
    void slotTreeWidgetItemClicked(QTreeWidgetItem* , int );
    QWidget* getWidget(QString widgetName);
   void currentItemChangedSlot(QTreeWidgetItem* currItem ,QTreeWidgetItem* prevItem);
   void run();

private:
    Ui::winIDENewTreeWidgetMainWindow* winIDENewTreeWidgetMainWindowUI;
    winIDENewXmlDomParser* _dIDEXmlDomParser;
    winIDEParseTclCommandUI* winIDEParseTclCommandUIInstance;
    QMap<QString, winIDENewParsedXmlStepData*> stepToStepDataMap;
    QMap<QString, QWidget*> classNameToClassInstanceMap;
    QMap<QWidget*, QString> widgetToClassNameMap;
    QMap<QString, int> classNameToWizardPageId;
    QMap<QString, QWizard*> classNameToWizardInstanceMap;

    QString _prevStringItem;
    QString _currStringItem;
    QWidget *_parentWidget;

    winIDENewParentWidgetForFlowNavigatorWidgets* winIDENewParentWidgetForFlowNavigatorWidgetsInst;
};

class winIDENewParsedXmlStepData
{
    QString _dName;
    QString _dIcon;
    QString _dTclProc;
    QString _dStatus;
    QString _dUiClass;
public:
    winIDENewParsedXmlStepData(const QString& name,
                            const QString& icon,
                            const QString& tclProc,
                            const QString& status,
                            const QString& uiClass
                            )
    {
       _dName = name;
       _dIcon = icon;
       _dTclProc = tclProc;
       _dStatus = status;
       _dUiClass = uiClass;
    }

    QString getUiClass()
    {
        return _dUiClass;
    }
    QString getTclProc()
    {
        return _dTclProc;
    }
    QString getName()
    {
        return _dName;
    }
    QString getIcon()
    {
        return _dIcon;
    }
};

class winIDENewXmlDomParser
{

public:
    winIDENewXmlDomParser(Ui::winIDENewTreeWidgetMainWindow*);
    bool readFile(const QString &fileName);
    QMap<QString, winIDENewParsedXmlStepData*> getStepToStepDataMap()
    {
        return stepToStepDataMap;
    }
    QTreeWidget* getTreeWidget() {return treeWidget;}

private:
    QMap<QString, winIDENewParsedXmlStepData*> stepToStepDataMap;
    Ui::winIDENewTreeWidgetMainWindow* m_winIDENewTreeWidgetMainWindowUI;
    void parseStepsElement(const QDomElement&, QTreeWidgetItem*);
    void parseStepsElementRecur(const QDomElement&, QTreeWidgetItem*);
    QTreeWidget *treeWidget;
    QString xmlRootElement;
};

#endif // WINIDENEWPARSEXMLWINDOW
#endif
