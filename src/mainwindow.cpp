/************************************************************************
 **
 **  @file   mainwindow.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "geometry/vspline.h"
#include "geometry/vequidistant.h"
#include "exception/vexceptionobjecterror.h"
#include "exception/vexceptionconversionerror.h"
#include "exception/vexceptionemptyparameter.h"
#include "exception/vexceptionwrongparameterid.h"
#include "exception/vexceptionuniqueid.h"
#include "version.h"

#include <QInputDialog>
#include <QtCore>
#include <QDebug>
#include <QMessageBox>
#include <QShowEvent>
#include <QScrollBar>
#include <QFileDialog>
#include <QXmlSchema>
#include <QXmlSchemaValidator>
#include <QSourceLocation>
#include <QAbstractMessageHandler>

class MessageHandler : public QAbstractMessageHandler
{
public:
    MessageHandler() : QAbstractMessageHandler(0), m_messageType(QtMsgType()), m_description(QString()),
        m_sourceLocation(QSourceLocation()){}
    inline QString statusMessage() const {return m_description;}
    inline qint64     line() const {return m_sourceLocation.line();}
    inline qint64     column() const {return m_sourceLocation.column();}
protected:
    virtual void handleMessage(QtMsgType type, const QString &description,
                               const QUrl &identifier, const QSourceLocation &sourceLocation)
    {
        Q_UNUSED(type);
        Q_UNUSED(identifier);

        m_messageType = type;
        m_description = description;
        m_sourceLocation = sourceLocation;
    }
private:
    QtMsgType       m_messageType;
    QString         m_description;
    QSourceLocation m_sourceLocation;
};

MainWindow::MainWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::MainWindow), pattern(0), doc(0), tool(Tool::ArrowTool), currentScene(0),
      sceneDraw(0), sceneDetails(0), mouseCoordinate(0), helpLabel(0), view(0), isInitialized(false), dialogTable(0),
      dialogTool(0), dialogHistory(0), comboBoxDraws(0), fileName(QString()), changeInFile(false),
      mode(Draw::Calculation), currentDrawIndex(0), currentToolBoxIndex(0), drawMode(true)
{
    ui->setupUi(this);
    qApp->installEventFilter(this);
    static const char * GENERIC_ICON_TO_CHECK = "document-open";
    if (QIcon::hasThemeIcon(GENERIC_ICON_TO_CHECK) == false)
    {
        //If there is no default working icon theme then we should
        //use an icon theme that we provide via a .qrc file
        //This case happens under Windows and Mac OS X
        //This does not happen under GNOME or KDE
        QIcon::setThemeName("win.icon.theme");
        ui->actionNew->setIcon(QIcon::fromTheme("document-new"));
        ui->actionOpen->setIcon(QIcon::fromTheme("document-open"));
        ui->actionSave->setIcon(QIcon::fromTheme("document-save"));
        ui->actionSaveAs->setIcon(QIcon::fromTheme("document-save-as"));
    }
    ToolBarOption();
    ToolBarDraws();
    sceneDraw = new VMainGraphicsScene();
    currentScene = sceneDraw;
    connect(sceneDraw, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    sceneDetails = new VMainGraphicsScene();
    connect(sceneDetails, &VMainGraphicsScene::mouseMove, this, &MainWindow::mouseMove);
    view = new VMainGraphicsView();
    ui->LayoutView->addWidget(view);
    view->setScene(currentScene);

    sceneDraw->setTransform(view->transform());
    sceneDetails->setTransform(view->transform());

    connect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
    QSizePolicy policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    policy.setHorizontalStretch(12);
    view->setSizePolicy(policy);
    helpLabel = new QLabel(tr("Create new pattern piece to start working."));
    ui->statusBar->addWidget(helpLabel);

    connect(ui->actionArrowTool, &QAction::triggered, this, &MainWindow::ActionAroowTool);
    connect(ui->actionDraw, &QAction::triggered, this, &MainWindow::ActionDraw);
    connect(ui->actionDetails, &QAction::triggered, this, &MainWindow::ActionDetails);
    connect(ui->actionNewDraw, &QAction::triggered, this, &MainWindow::ActionNewDraw);
    connect(ui->actionOptionDraw, &QAction::triggered, this, &MainWindow::OptionDraw);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::ActionSaveAs);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::ActionSave);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::ActionOpen);
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::ActionNew);
    connect(ui->actionTable, &QAction::triggered, this, &MainWindow::ActionTable);
    connect(ui->toolButtonEndLine, &QToolButton::clicked, this, &MainWindow::ToolEndLine);
    connect(ui->toolButtonLine, &QToolButton::clicked, this, &MainWindow::ToolLine);
    connect(ui->toolButtonAlongLine, &QToolButton::clicked, this, &MainWindow::ToolAlongLine);
    connect(ui->toolButtonShoulderPoint, &QToolButton::clicked, this, &MainWindow::ToolShoulderPoint);
    connect(ui->toolButtonNormal, &QToolButton::clicked, this, &MainWindow::ToolNormal);
    connect(ui->toolButtonBisector, &QToolButton::clicked, this, &MainWindow::ToolBisector);
    connect(ui->toolButtonLineIntersect, &QToolButton::clicked, this, &MainWindow::ToolLineIntersect);
    connect(ui->toolButtonSpline, &QToolButton::clicked, this, &MainWindow::ToolSpline);
    connect(ui->toolButtonArc, &QToolButton::clicked, this, &MainWindow::ToolArc);
    connect(ui->toolButtonSplinePath, &QToolButton::clicked, this, &MainWindow::ToolSplinePath);
    connect(ui->toolButtonPointOfContact, &QToolButton::clicked, this, &MainWindow::ToolPointOfContact);
    connect(ui->toolButtonNewDetail, &QToolButton::clicked, this, &MainWindow::ToolDetail);
    connect(ui->toolButtonHeight, &QToolButton::clicked, this, &MainWindow::ToolHeight);
    connect(ui->toolButtonTriangle, &QToolButton::clicked, this, &MainWindow::ToolTriangle);
    connect(ui->toolButtonPointOfIntersection, &QToolButton::clicked, this, &MainWindow::ToolPointOfIntersection);
    connect(ui->toolButtonSplineCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSpline);
    connect(ui->toolButtonSplinePathCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutSplinePath);
    connect(ui->toolButtonUnionDetails, &QToolButton::clicked, this, &MainWindow::ToolUnionDetails);
    connect(ui->toolButtonArcCutPoint, &QToolButton::clicked, this, &MainWindow::ToolCutArc);

    pattern = new VContainer();

    doc = new VDomDocument(pattern, comboBoxDraws, &mode);
    doc->CreateEmptyFile();
    connect(doc, &VDomDocument::haveChange, this, &MainWindow::haveChange);

    //Autosaving file each 5 minutes
    QTimer *timer = new QTimer(this);
    timer->setTimerType(Qt::VeryCoarseTimer);
    connect(timer, &QTimer::timeout, this, &MainWindow::AutoSavePattern);
    timer->start(300000);

    connect(ui->actionAbout_Qt, &QAction::triggered, this, &MainWindow::AboutQt);
    connect(ui->actionAbout_Valentina, &QAction::triggered, this, &MainWindow::About);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::close);

    ui->toolBox->setCurrentIndex(0);
}

void MainWindow::ActionNewDraw()
{
    QString nameDraw;
    bool bOk;
    qint32 index;
    QString nDraw = QString(tr("Pattern piece %1")).arg(comboBoxDraws->count()+1);
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(nDraw);
    while (1)
    {
        bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty())
        {
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if (index != -1)
        {//we already have this name
            qCritical()<<tr("Error. Pattern piece of same label already exists.");
        }
        else
        {
            break;
        }
    }
    delete dlg;
    bOk = doc->appendDraw(nameDraw);
    if (bOk == false)
    {
        qCritical()<<tr("Error creating pattern with the name ")<<nameDraw<<".";
        return;
    }
    disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
               this, &MainWindow::currentDrawChanged);
    comboBoxDraws->addItem(nameDraw);
    index = comboBoxDraws->findText(nameDraw);
    if ( index != -1 )
    { // -1 for not found
        comboBoxDraws->setCurrentIndex(index);
        currentDrawChanged( index );
    }
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this,
            &MainWindow::currentDrawChanged);
    pattern->ClearGObjects();
    //Create single point
    qint64 id = pattern->AddGObject(new VPointF(toPixel((10+comboBoxDraws->count()*5)), toPixel(10), "А", 5, 10));
    VToolSinglePoint *spoint = new VToolSinglePoint(doc, pattern, id, Tool::FromGui);
    sceneDraw->addItem(spoint);
    connect(spoint, &VToolPoint::ChoosedTool, sceneDraw, &VMainGraphicsScene::ChoosedItem);
    connect(sceneDraw, &VMainGraphicsScene::NewFactor, spoint, &VToolSinglePoint::SetFactor);
    QHash<qint64, VDataTool*>* tools = doc->getTools();
    tools->insert(id, spoint);
    VDrawTool::AddRecord(id, Tool::SinglePointTool, doc);
    SetEnableTool(true);
    SetEnableWidgets(true);
    changeInFile = true;
}

void MainWindow::OptionDraw()
{
    QString nameDraw;
    qint32 index;
    QString nDraw = doc->GetNameActivDraw();
    QInputDialog *dlg = new QInputDialog(this);
    dlg->setInputMode( QInputDialog::TextInput );
    dlg->setLabelText(tr("Pattern piece:"));
    dlg->setTextEchoMode(QLineEdit::Normal);
    dlg->setWindowTitle(tr("Enter a new label for the pattern piece."));
    dlg->resize(300, 100);
    dlg->setTextValue(nDraw);
    while (1)
    {
        bool bOk = dlg->exec();
        nameDraw = dlg->textValue();
        if (bOk == false || nameDraw.isEmpty())
        {
            delete dlg;
            return;
        }
        index = comboBoxDraws->findText(nameDraw);
        if (index != -1)
        {//we already have this name
            qCritical()<<tr("Error. Pattern piece of same name already exists.");
        }
        else
        {
            break;
        }
    }
    delete dlg;
    index = comboBoxDraws->findText(doc->GetNameActivDraw());
    if (doc->SetNameDraw(nameDraw))
    {
        comboBoxDraws->setItemText(index, nameDraw);
    }
    else
    {
        QMessageBox::warning(this, tr("Error saving change!!!"), tr("Can't save new label of pattern piece"));
    }

}

template <typename Dialog, typename Func>
void MainWindow::SetToolButton(bool checked, Tool::Tools t, const QString &cursor, const QString &toolTip,
                               Func closeDialogSlot)
{
    if (checked)
    {
        CancelTool();
        tool = t;
        QPixmap pixmap(cursor);
        QCursor cur(pixmap, 2, 3);
        view->setCursor(cur);
        helpLabel->setText(toolTip);
        dialogTool = new Dialog(pattern, this);
        Q_CHECK_PTR(dialogTool);
        connect(this, &MainWindow::ApplicationDeactivate, dialogTool, &DialogTool::ApplicationDeactivate);
        connect(currentScene, &VMainGraphicsScene::ChoosedObject, dialogTool, &DialogTool::ChoosedObject);
        connect(dialogTool, &DialogTool::DialogClosed, this, closeDialogSlot);
        connect(dialogTool, &DialogTool::ToolTip, this, &MainWindow::ShowToolTip);
        connect(doc, &VDomDocument::FullUpdateFromFile, dialogTool, &DialogTool::UpdateList);
    }
    else
    {
        if (QToolButton *tButton = qobject_cast< QToolButton * >(this->sender()))
        {
            Q_CHECK_PTR(tButton);
            tButton->setChecked(true);
        }
    }
}

template <typename DrawTool>
void MainWindow::ClosedDialog(int result)
{
    Q_CHECK_PTR(dialogTool);
    if (result == QDialog::Accepted)
    {
        DrawTool::Create(dialogTool, currentScene, doc, pattern);
    }
    ArrowTool();
}

void MainWindow::ToolEndLine(bool checked)
{
    SetToolButton<DialogEndLine>(checked, Tool::EndLineTool, ":/cursor/endline_cursor.png", tr("Select point"),
                                 &MainWindow::ClosedDialogEndLine);
}

void MainWindow::ClosedDialogEndLine(int result)
{
    ClosedDialog<VToolEndLine>(result);
}

void MainWindow::ToolLine(bool checked)
{
    SetToolButton<DialogTool>(checked, Tool::LineTool, ":/cursor/line_cursor.png", tr("Select first point"),
                              &MainWindow::ClosedDialogLine);
}

void MainWindow::ClosedDialogLine(int result)
{
    ClosedDialog<VToolLine>(result);
}

void MainWindow::ToolAlongLine(bool checked)
{
    SetToolButton<DialogAlongLine>(checked, Tool::AlongLineTool, ":/cursor/alongline_cursor.png", tr("Select point"),
                                   &MainWindow::ClosedDialogAlongLine);
}

void MainWindow::ClosedDialogAlongLine(int result)
{
    ClosedDialog<VToolAlongLine>(result);
}

void MainWindow::ToolShoulderPoint(bool checked)
{
    SetToolButton<DialogShoulderPoint>(checked, Tool::ShoulderPointTool, ":/cursor/shoulder_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogShoulderPoint);
}

void MainWindow::ClosedDialogShoulderPoint(int result)
{
    ClosedDialog<VToolShoulderPoint>(result);
}

void MainWindow::ToolNormal(bool checked)
{
    SetToolButton<DialogNormal>(checked, Tool::NormalTool, ":/cursor/normal_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogNormal);
}

void MainWindow::ClosedDialogNormal(int result)
{
    ClosedDialog<VToolNormal>(result);
}

void MainWindow::ToolBisector(bool checked)
{
    SetToolButton<DialogBisector>(checked, Tool::BisectorTool, ":/cursor/bisector_cursor.png",
                  tr("Select first point of angle"), &MainWindow::ClosedDialogBisector);
}

void MainWindow::ClosedDialogBisector(int result)
{
    ClosedDialog<VToolBisector>(result);
}

void MainWindow::ToolLineIntersect(bool checked)
{
    SetToolButton<DialogLineIntersect>(checked, Tool::LineIntersectTool, ":/cursor/intersect_cursor.png",
                  tr("Select first point of first line"), &MainWindow::ClosedDialogLineIntersect);
}

void MainWindow::ClosedDialogLineIntersect(int result)
{
    ClosedDialog<VToolLineIntersect>(result);
}

void MainWindow::ToolSpline(bool checked)
{
    SetToolButton<DialogSpline>(checked, Tool::SplineTool, ":/cursor/spline_cursor.png",
                  tr("Select first point curve"), &MainWindow::ClosedDialogSpline);
}

void MainWindow::ClosedDialogSpline(int result)
{
    ClosedDialog<VToolSpline>(result);
}

void MainWindow::ToolCutSpline(bool checked)
{
    SetToolButton<DialogCutSpline>(checked, Tool::CutSplineTool, ":/cursor/spline_cut_point_cursor.png",
                  tr("Select simple curve"), &MainWindow::ClosedDialogCutSpline);
}

void MainWindow::ClosedDialogCutSpline(int result)
{
    ClosedDialog<VToolCutSpline>(result);
}

void MainWindow::ToolArc(bool checked)
{
    SetToolButton<DialogArc>(checked, Tool::ArcTool, ":/cursor/arc_cursor.png",
                  tr("Select point of center of arc"), &MainWindow::ClosedDialogArc);
}

void MainWindow::ClosedDialogArc(int result)
{
    ClosedDialog<VToolArc>(result);
}

void MainWindow::ToolSplinePath(bool checked)
{
    SetToolButton<DialogSplinePath>(checked, Tool::SplinePathTool, ":/cursor/splinepath_cursor.png",
                  tr("Select point of curve path"), &MainWindow::ClosedDialogSplinePath);
}

void MainWindow::ClosedDialogSplinePath(int result)
{
    ClosedDialog<VToolSplinePath>(result);
}

void MainWindow::ToolCutSplinePath(bool checked)
{
    SetToolButton<DialogCutSplinePath>(checked, Tool::CutSplinePathTool, ":/cursor/splinepath_cut_point_cursor.png",
                  tr("Select curve path"), &MainWindow::ClosedDialogCutSplinePath);
}

void MainWindow::ClosedDialogCutSplinePath(int result)
{
    ClosedDialog<VToolCutSplinePath>(result);
}

void MainWindow::ToolPointOfContact(bool checked)
{
    SetToolButton<DialogPointOfContact>(checked, Tool::PointOfContact, ":/cursor/pointcontact_cursor.png",
                  tr("Select first point of line"), &MainWindow::ClosedDialogPointOfContact);
}

void MainWindow::ClosedDialogPointOfContact(int result)
{
    ClosedDialog<VToolPointOfContact>(result);
}

void MainWindow::ToolDetail(bool checked)
{
    SetToolButton<DialogDetail>(checked, Tool::Detail, "://cursor/new_detail_cursor.png",
                                tr("Select points, arcs, curves clockwise."), &MainWindow::ClosedDialogDetail);
}

void MainWindow::ClosedDialogDetail(int result)
{
    if (result == QDialog::Accepted)
    {
        VToolDetail::Create(dialogTool, sceneDetails, doc, pattern);
    }
    ArrowTool();
    doc->FullUpdateTree();
}

void MainWindow::ToolHeight(bool checked)
{
    SetToolButton<DialogHeight>(checked, Tool::Height, ":/cursor/height_cursor.png", tr("Select base point"),
                                &MainWindow::ClosedDialogHeight);
}

void MainWindow::ClosedDialogHeight(int result)
{
    ClosedDialog<VToolHeight>(result);
}

void MainWindow::ToolTriangle(bool checked)
{
    SetToolButton<DialogTriangle>(checked, Tool::Triangle, ":/cursor/triangle_cursor.png",
                                  tr("Select first point of axis"), &MainWindow::ClosedDialogTriangle);
}

void MainWindow::ClosedDialogTriangle(int result)
{
    ClosedDialog<VToolTriangle>(result);
}

void MainWindow::ToolPointOfIntersection(bool checked)
{
    SetToolButton<DialogPointOfIntersection>(checked, Tool::PointOfIntersection, ":/cursor/pointofintersect_cursor.png",
                  tr("Select point vertically"), &MainWindow::ClosedDialogPointOfIntersection);
}

void MainWindow::ClosedDialogPointOfIntersection(int result)
{
    ClosedDialog<VToolPointOfIntersection>(result);
}

void MainWindow::ToolUnionDetails(bool checked)
{
    SetToolButton<DialogUnionDetails>(checked, Tool::UnionDetails, ":/cursor/union_cursor.png", tr("Select detail"),
                                      &MainWindow::ClosedDialogUnionDetails);
    //Must disconnect this signal here.
    disconnect(doc, &VDomDocument::FullUpdateFromFile, dialogTool, &DialogTool::UpdateList);
}

void MainWindow::ClosedDialogUnionDetails(int result)
{
    ClosedDialog<VToolUnionDetails>(result);
    doc->FullUpdateTree();
}

void MainWindow::ToolCutArc(bool checked)
{
    SetToolButton<DialogCutArc>(checked, Tool::CutArcTool, ":/cursor/arc_cut_cursor.png", tr("Select arc"),
                  &MainWindow::ClosedDialogCutArc);
}

void MainWindow::ClosedDialogCutArc(int result)
{
    ClosedDialog<VToolCutArc>(result);
}

void MainWindow::About()
{
    QString fullName = QString("Valentina %1").arg(APP_VERSION);
    QString qtBase(tr("Based on Qt %2 (32 bit)").arg(QT_VERSION_STR));
    QString buildOn(tr("Built on %3 at %4").arg(__DATE__).arg(__TIME__));
    QString about = QString(tr("<h1>%1</h1> %2 <br/><br/> %3 <br/><br/> %4")).arg(fullName).arg(qtBase).arg(
                buildOn).arg(WARRANTY);
    QMessageBox::about(this, tr("About Valentina"), about);
}

void MainWindow::AboutQt()
{
    QMessageBox::aboutQt(this, tr("About Qt"));
}

void MainWindow::ShowToolTip(const QString &toolTip)
{
    helpLabel->setText(toolTip);
}

void MainWindow::tableClosed()
{
    show();
    MinimumScrollBar();
}

void MainWindow::showEvent( QShowEvent *event )
{
    QMainWindow::showEvent( event );
    if ( event->spontaneous() )
    {
        return;
    }

    if (isInitialized)
    {
        return;
    }
    // do your init stuff here

    MinimumScrollBar();

    isInitialized = true;//first show windows are held
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (changeInFile == true)
    {
        QMessageBox msgBox(this);
        msgBox.setText(tr("The pattern has been modified."));
        msgBox.setInformativeText(tr("Do you want to save your changes?"));
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setIcon(QMessageBox::Question);
        int ret = msgBox.exec();
        switch (ret)
        {
            case QMessageBox::Save:
                // Save was clicked
                if (fileName.isEmpty())
                {
                    ActionSaveAs();
                }
                else
                {
                    ActionSave();
                }
                if (changeInFile)
                {
                    // We did't save file
                    event->ignore();
                }
                else
                {
                    // We have successfully saved the file
                    event->accept();
                }
                break;
            case QMessageBox::Discard:
                // Don't Save was clicked
                event->accept();
                break;
            case QMessageBox::Cancel:
                // Cancel was clicked
                event->ignore();
                break;
            default:
                // should never be reached
                event->accept();
                break;
        }
    }
}

void MainWindow::ToolBarOption()
{
    QLabel * labelGrowth = new QLabel;
    labelGrowth->setText(tr("Height: "));
    ui->toolBarOption->addWidget(labelGrowth);

    QStringList list;
    list <<"92"<<"98"<<"104"<<"110"<<"116"<<"122"<<"128"<<"134"<<"140"<<"146"<<"152"<<"158"<<"164"<<"170"<<"176"
         <<"182"<<"188";
    QComboBox *comboBoxGrow = new QComboBox;
    comboBoxGrow->clear();
    comboBoxGrow->addItems(list);
    comboBoxGrow->setCurrentIndex(14);
    ui->toolBarOption->addWidget(comboBoxGrow);
    connect(comboBoxGrow, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedHeight);

    QLabel * labelSize = new QLabel;
    labelSize->setText(tr(" Size: "));
    ui->toolBarOption->addWidget(labelSize);

    list.clear();
    list <<"22"<<"24"<<"26"<<"28"<<"30"<<"32"<<"34"<<"36"<<"38"<<"40"<<"42"<<"44"<<"46"<<"48"<<"50"<<"52"<<"54"<<"56";
    QComboBox *comboBoxSize = new QComboBox;
    comboBoxSize->clear();
    comboBoxSize->addItems(list);
    comboBoxSize->setCurrentIndex(14);
    ui->toolBarOption->addWidget(comboBoxSize);
    connect(comboBoxSize, static_cast<void (QComboBox::*)(const QString &)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::ChangedSize);

    ui->toolBarOption->addSeparator();

    mouseCoordinate = new QLabel;
    mouseCoordinate ->setText("0, 0");
    ui->toolBarOption->addWidget(mouseCoordinate);

}

void MainWindow::ToolBarDraws()
{
    QLabel * labelNameDraw = new QLabel;
    labelNameDraw ->setText(tr("Pattern Piece: "));
    ui->toolBarDraws->addWidget(labelNameDraw);

    comboBoxDraws = new QComboBox;
    ui->toolBarDraws->addWidget(comboBoxDraws);
    comboBoxDraws->setSizeAdjustPolicy(QComboBox::AdjustToContents);
    connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::currentDrawChanged);

    ui->toolBarDraws->addAction(ui->actionOptionDraw);
    ui->actionOptionDraw->setEnabled(false);

    ui->toolBarDraws->addAction(ui->actionTable);
    ui->actionTable->setEnabled(false);

    ui->toolBarDraws->addAction(ui->actionHistory);
    ui->actionHistory->setEnabled(false);
    connect(ui->actionHistory, &QAction::triggered, this, &MainWindow::ActionHistory);

    ui->toolBarDraws->addAction(ui->actionLayout);
    connect(ui->actionLayout, &QAction::triggered, this, &MainWindow::ActionLayout);
    ui->actionLayout->setEnabled(false);
}

void MainWindow::currentDrawChanged( int index )
{
    if (index != -1)
    {
        doc->setCurrentData();
        doc->ChangeActivDraw(comboBoxDraws->itemText(index));
        if (drawMode)
        {
            ArrowTool();
            qint64 id = doc->SPointActiveDraw();
            if (id != 0)
            {
                const VPointF *p = pattern->GeometricObject<const VPointF *>(id);
                view->centerOn(p->toQPointF());
            }
        }
    }
}

void MainWindow::mouseMove(const QPointF &scenePos)
{
    QString string = QString("%1, %2")
                            .arg(static_cast<qint32>(toMM(scenePos.x())))
                            .arg(static_cast<qint32>(toMM(scenePos.y())));
    mouseCoordinate->setText(string);
}

void MainWindow::CancelTool()
{
    delete dialogTool;
    dialogTool = 0;
    switch ( tool )
    {
        case Tool::ArrowTool:
            ui->actionArrowTool->setChecked(false);
            helpLabel->setText("");
            break;
        case Tool::SinglePointTool:
            Q_UNREACHABLE();
            //Nothing to do here because we can't create this tool from main window.
            break;
        case Tool::EndLineTool:
            ui->toolButtonEndLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::LineTool:
            ui->toolButtonLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearFocus();
            break;
        case Tool::AlongLineTool:
            ui->toolButtonAlongLine->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::ShoulderPointTool:
            ui->toolButtonShoulderPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::NormalTool:
            ui->toolButtonNormal->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::BisectorTool:
            ui->toolButtonBisector->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::LineIntersectTool:
            ui->toolButtonLineIntersect->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::SplineTool:
            ui->toolButtonSpline->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::ArcTool:
            ui->toolButtonArc->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::SplinePathTool:
            ui->toolButtonSplinePath->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::PointOfContact:
            ui->toolButtonPointOfContact->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::Detail:
            ui->toolButtonNewDetail->setChecked(false);
            break;
        case Tool::Height:
            ui->toolButtonHeight->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::Triangle:
            ui->toolButtonTriangle->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::PointOfIntersection:
            ui->toolButtonPointOfIntersection->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutSplineTool:
            ui->toolButtonSplineCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutSplinePathTool:
            ui->toolButtonSplinePathCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::UnionDetails:
            ui->toolButtonUnionDetails->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        case Tool::CutArcTool:
            ui->toolButtonArcCutPoint->setChecked(false);
            currentScene->setFocus(Qt::OtherFocusReason);
            currentScene->clearSelection();
            break;
        default:
            qWarning()<<"Got wrong tool type. Ignored.";
            break;
    }
}

void  MainWindow::ArrowTool()
{
    CancelTool();
    ui->actionArrowTool->setChecked(true);
    tool = Tool::ArrowTool;
    QCursor cur(Qt::ArrowCursor);
    view->setCursor(cur);
    helpLabel->setText("");
}

void MainWindow::ActionAroowTool()
{
    ArrowTool();
}

void MainWindow::keyPressEvent ( QKeyEvent * event )
{
    switch (event->key())
    {
        case Qt::Key_Escape:
            ArrowTool();
            break;
        default:
            break;
    }
    QMainWindow::keyPressEvent ( event );
}

void MainWindow::SaveCurrentScene()
{
    /*Save transform*/
    currentScene->setTransform(view->transform());
    /*Save scroll bars value for previous scene.*/
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    currentScene->setHorScrollBar(horScrollBar->value());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    currentScene->setVerScrollBar(verScrollBar->value());
}

void MainWindow::RestoreCurrentScene()
{
    /*Set transform for current scene*/
    view->setTransform(currentScene->transform());
    /*Set value for current scene scroll bar.*/
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(currentScene->getHorScrollBar());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(currentScene->getVerScrollBar());
}

void MainWindow::ActionDraw(bool checked)
{
    if (checked)
    {
        ui->actionDetails->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDraw;
        view->setScene(currentScene);
        connect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        mode = Draw::Calculation;
        comboBoxDraws->setEnabled(true);
        comboBoxDraws->setCurrentIndex(currentDrawIndex);//restore current pattern peace
        drawMode = true;

        SetEnableTool(true);
        doc->setCurrentData();
        ui->toolBox->setCurrentIndex(currentToolBoxIndex);

        ui->actionHistory->setEnabled(true);
        ui->actionLayout->setEnabled(false);
        ui->actionOptionDraw->setEnabled(true);
        ui->actionNewDraw->setEnabled(true);
    }
    else
    {
        ui->actionDraw->setChecked(true);
    }
}

void MainWindow::ActionDetails(bool checked)
{
    if (checked)
    {
        ui->actionDraw->setChecked(false);
        SaveCurrentScene();

        currentScene = sceneDetails;
        view->setScene(sceneDetails);
        disconnect(view, &VMainGraphicsView::NewFactor, sceneDraw, &VMainGraphicsScene::SetFactor);
        RestoreCurrentScene();

        drawMode = false;
        currentDrawIndex = comboBoxDraws->currentIndex();//save current pattern peace
        comboBoxDraws->setCurrentIndex(comboBoxDraws->count()-1);
        comboBoxDraws->setEnabled(false);


        mode = Draw::Modeling;
        SetEnableTool(true);
        currentToolBoxIndex = ui->toolBox->currentIndex();
        ui->toolBox->setCurrentIndex(4);

        ui->actionHistory->setEnabled(false);
        ui->actionLayout->setEnabled(true);
        ui->actionOptionDraw->setEnabled(false);
        ui->actionNewDraw->setEnabled(false);
    }
    else
    {
        ui->actionDetails->setChecked(true);
    }
}

void MainWindow::ActionSaveAs()
{
    QString filters(tr("Pattern files (*.val)"));
    QString dir = QDir::homePath() + tr("/pattern.val");
    QString fName = QFileDialog::getSaveFileName(this, tr("Save as"), dir, filters);

    if (fName.isEmpty())
    {
        return;
    }
    QFileInfo f( fName );
    if (f.suffix().isEmpty() && f.suffix() != "val")
    {
        fName += ".val";
    }
    fileName = fName;

    ActionSave();
}

void MainWindow::ActionSave()
{
    if (fileName.isEmpty() == false)
    {
        bool result = SafeSaveing(fileName);
        if (result)
        {
            ui->actionSave->setEnabled(false);
            changeInFile = false;
            QFileInfo info(fileName);
            QString title(info.fileName());
            title.append("-Valentina");
            setWindowTitle(title);
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Error!"));
            msgBox.setText(tr("Error saving file. Can't save file."));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.exec();
        }
    }
}

void MainWindow::ActionOpen()
{
    QString filter(tr("Pattern files (*.val)"));
    QString fName = QFileDialog::getOpenFileName(this, tr("Open file"), QDir::homePath(), filter);
    if (fName.isEmpty())
    {
        return;
    }
    if (fileName.isEmpty() && changeInFile == false)
    {
        OpenPattern(fName);

        VAbstractTool::NewSceneRect(sceneDraw, view);
        VAbstractTool::NewSceneRect(sceneDetails, view);
    }
    else
    {
        /*Open new copy application*/
        QProcess *v = new QProcess(this);
        QStringList arguments;
        arguments << "-o" << fName;
        v->startDetached(QCoreApplication::applicationFilePath (), arguments);
        delete v;
    }
}

void MainWindow::Clear()
{
    setWindowTitle("Valentina");
    fileName.clear();
    pattern->Clear();
    doc->clear();
    sceneDraw->clear();
    sceneDetails->clear();
    CancelTool();
    comboBoxDraws->clear();
    fileName.clear();
    ui->actionOptionDraw->setEnabled(false);
    ui->actionSave->setEnabled(false);
    SetEnableTool(false);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wswitch-enum"
#endif
    switch(event->type())
    {
        case(QEvent::ApplicationDeactivate):
            emit ApplicationDeactivate(true);
            break;
        case(QEvent::ApplicationActivate):
            emit ApplicationDeactivate(false);
            break;
        default:
            break;
    }
#ifdef Q_CC_GNU
    #pragma GCC diagnostic pop
#endif
    return QWidget::eventFilter(object, event);
}

void MainWindow::ActionNew()
{
    QProcess *v = new QProcess(this);
    v->startDetached(QCoreApplication::applicationFilePath ());
    delete v;
}

void MainWindow::haveChange()
{
    if (fileName.isEmpty() == false)
    {
        ui->actionSave->setEnabled(true);
        changeInFile = true;
        QFileInfo info(fileName);
        QString title(info.fileName());
        title.append("*-Valentina");
        setWindowTitle(title);
    }
}

void MainWindow::ChangedSize(const QString & text)
{
    qint32 size = text.toInt();
    pattern->SetSize(size*10);
    doc->FullUpdateTree();
}

void MainWindow::ChangedHeight(const QString &text)
{
    qint32 growth = text.toInt();
    pattern->SetGrowth(growth*10);
    doc->FullUpdateTree();
}

void MainWindow::SetEnableWidgets(bool enable)
{
    ui->actionSaveAs->setEnabled(enable);
    ui->actionDraw->setEnabled(enable);
    ui->actionDetails->setEnabled(enable);
    ui->actionOptionDraw->setEnabled(enable);
    if (enable == true && fileName.isEmpty() == false)
    {
        ui->actionSave->setEnabled(enable);
    }
    ui->actionTable->setEnabled(enable);
    ui->actionHistory->setEnabled(enable);
}

void MainWindow::ActionTable(bool checked)
{
    if (checked)
    {
        dialogTable = new DialogIncrements(pattern, doc, this);
        connect(dialogTable, &DialogIncrements::DialogClosed, this,
                &MainWindow::ClosedActionTable);
        dialogTable->show();
    }
    else
    {
        ui->actionTable->setChecked(true);
        dialogTable->activateWindow();
    }
}

void MainWindow::ClosedActionTable()
{
    ui->actionTable->setChecked(false);
    delete dialogTable;
}

void MainWindow::ActionHistory(bool checked)
{
    if (checked)
    {
        dialogHistory = new DialogHistory(pattern, doc, this);
        dialogHistory->setWindowFlags(Qt::Window);
        connect(dialogHistory, &DialogHistory::DialogClosed, this,
                &MainWindow::ClosedActionHistory);
        dialogHistory->show();
    }
    else
    {
        ui->actionHistory->setChecked(true);
        dialogHistory->activateWindow();
    }
}

void MainWindow::ActionLayout(bool checked)
{
    Q_UNUSED(checked);
    hide();
    QVector<VItem*> listDetails;
    const QHash<qint64, VDetail> *details = pattern->DataDetails();
    QHashIterator<qint64, VDetail> idetail(*details);
    while (idetail.hasNext())
    {
        idetail.next();
        QPainterPath path = VEquidistant().ContourPath(idetail.key(), pattern);
        listDetails.append(new VItem(path, listDetails.size()));
    }
    emit ModelChosen(listDetails, fileName);
}

void MainWindow::ClosedActionHistory()
{
    ui->actionHistory->setChecked(false);
    delete dialogHistory;
}

void MainWindow::SetEnableTool(bool enable)
{
    bool drawTools = false;
    bool modelingTools = false;
    if (mode == Draw::Calculation)
    {
        drawTools = enable;
    }
    else
    {
        modelingTools = enable;
    }
    //Drawing Tools
    ui->toolButtonEndLine->setEnabled(drawTools);
    ui->toolButtonLine->setEnabled(drawTools);
    ui->toolButtonAlongLine->setEnabled(drawTools);
    ui->toolButtonShoulderPoint->setEnabled(drawTools);
    ui->toolButtonNormal->setEnabled(drawTools);
    ui->toolButtonBisector->setEnabled(drawTools);
    ui->toolButtonLineIntersect->setEnabled(drawTools);
    ui->toolButtonSpline->setEnabled(drawTools);
    ui->toolButtonArc->setEnabled(drawTools);
    ui->toolButtonSplinePath->setEnabled(drawTools);
    ui->toolButtonPointOfContact->setEnabled(drawTools);
    ui->toolButtonNewDetail->setEnabled(drawTools);
    ui->toolButtonHeight->setEnabled(drawTools);
    ui->toolButtonTriangle->setEnabled(drawTools);
    ui->toolButtonPointOfIntersection->setEnabled(drawTools);
    ui->toolButtonSplineCutPoint->setEnabled(drawTools);
    ui->toolButtonSplinePathCutPoint->setEnabled(drawTools);
    ui->toolButtonArcCutPoint->setEnabled(drawTools);

    //Modeling Tools
    ui->toolButtonUnionDetails->setEnabled(modelingTools);
}

void MainWindow::MinimumScrollBar()
{
    QScrollBar *horScrollBar = view->horizontalScrollBar();
    horScrollBar->setValue(horScrollBar->minimum());
    QScrollBar *verScrollBar = view->verticalScrollBar();
    verScrollBar->setValue(verScrollBar->minimum());
}

bool MainWindow::ValidatePattern(const QString &schema, const QString &fileName, QString &errorMsg, qint64 &errorLine,
                                 qint64 &errorColumn) const
{
    QFile pattern(fileName);
    if (pattern.open(QIODevice::ReadOnly) == false)
    {
        errorMsg = QString(tr("Can't open pattern file."));
        errorLine = -1;
        errorColumn = -1;
        return false;
    }
    if (schema.isEmpty())
    {
        errorMsg = QString(tr("Empty schema path."));
        errorLine = -1;
        errorColumn = -1;
        return false;
    }
    QFile fileSchema(schema);
    if (fileSchema.open(QIODevice::ReadOnly) == false)
    {
        errorMsg = QString(tr("Can't open schema file."));
        errorLine = -1;
        errorColumn = -1;
        return false;
    }

    MessageHandler messageHandler;
    QXmlSchema sch;
    sch.setMessageHandler(&messageHandler);
    sch.load(&fileSchema, QUrl::fromLocalFile(fileSchema.fileName()));

    bool errorOccurred = false;
    if (sch.isValid() == false)
    {
        errorOccurred = true;
    }
    else
    {
        QXmlSchemaValidator validator(sch);
        if (validator.validate(&pattern, QUrl::fromLocalFile(pattern.fileName())) == false)
        {
            errorOccurred = true;
        }
    }

    if (errorOccurred)
    {
        errorMsg = messageHandler.statusMessage();
        errorLine = messageHandler.line();
        errorColumn = messageHandler.column();
        return false;
    }
    else
    {
        return true;
    }
}

bool MainWindow::SafeSaveing(const QString &fileName) const
{
    try
    {
        doc->TestUniqueId();
    }
    catch (const VExceptionUniqueId &e)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("Error!"));
        msgBox.setText(tr("Error no unique id."));
        msgBox.setInformativeText(e.ErrorMessage());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setDetailedText(e.DetailedInformation());
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();
        return false;
    }
    if (fileName.isEmpty())
    {
        qWarning()<<tr("Got empty file name.");
        return false;
    }
    //Writing in temporary file
    QFileInfo tempInfo(fileName);
    QString temp = tempInfo.absolutePath() + "/" + tempInfo.baseName() + ".tmp";
    qDebug()<<"file "<<fileName<<"temp file "<<temp;
    QFile tempFile(temp);
    if (tempFile.open(QIODevice::WriteOnly| QIODevice::Truncate))
    {
        const int Indent = 4;
        QTextStream out(&tempFile);
        doc->save(out, Indent);
        tempFile.close();
    }
    //Replace temp file our
    bool result = false;
    QFile patternFile(fileName);
    // We need here temporary file because we need restore pattern after error of copying temp file.
    QTemporaryFile tempOfPattern;
    if (tempOfPattern.open())
    {
        patternFile.copy(tempOfPattern.fileName());
    }
    if ( patternFile.exists() == false || patternFile.remove() )
    {
        if ( tempFile.copy(patternFile.fileName()) == false )
        {
            qCritical()<<tr("Could not copy temp file to pattern file")<<Q_FUNC_INFO;
            tempOfPattern.copy(fileName);
            result = false;
        }
        else
        {
            result = true;
        }
    }
    else
    {
        qCritical()<<tr("Could not remove pattern file")<<Q_FUNC_INFO;
        result = false;
    }
    if (result)
    {
        tempFile.remove();
    }
    return result;
}

void MainWindow::AutoSavePattern()
{
    if (fileName.isEmpty() == false && changeInFile == true)
    {
        bool result = SafeSaveing(fileName);
        if (result)
        {
            ui->actionSave->setEnabled(false);
            changeInFile = false;
            QFileInfo info(fileName);
            QString title(info.fileName());
            title.append("-Valentina");
            setWindowTitle(title);
        }
    }
}

MainWindow::~MainWindow()
{
    CancelTool();
    delete ui;

    delete pattern;
    delete doc;
    delete sceneDetails;
    delete sceneDraw;
}

void MainWindow::OpenPattern(const QString &fileName)
{
    if (fileName.isEmpty())
    {
        qWarning()<<tr("Can't open pattern file. File name is empty")<<Q_FUNC_INFO;
        return;
    }
    QFile file(fileName);
    QString errorMsg;
    qint64 errorLine = 0;
    qint64 errorColumn = 0;
    if (file.open(QIODevice::ReadOnly))
    {
        if (ValidatePattern("://schema/pattern.xsd", fileName, errorMsg, errorLine, errorColumn))
        {
            qint32 errorLine = 0;
            qint32 errorColumn = 0;
            if (doc->setContent(&file, &errorMsg, &errorLine, &errorColumn))
            {
                disconnect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                           this, &MainWindow::currentDrawChanged);
                try
                {
                    doc->Parse(Document::FullParse, sceneDraw, sceneDetails);
                }
                catch (const VExceptionObjectError &e)
                {
                    e.CriticalMessageBox(tr("Error parsing file."));
                    file.close();
                    Clear();
                    return;
                }
                catch (const VExceptionConversionError &e)
                {
                    e.CriticalMessageBox(tr("Error can't convert value."));
                    file.close();
                    Clear();
                    return;
                }
                catch (const VExceptionEmptyParameter &e)
                {
                    e.CriticalMessageBox(tr("Error empty parameter."));
                    file.close();
                    Clear();
                    return;
                }
                catch (const VExceptionWrongParameterId &e)
                {
                    e.CriticalMessageBox(tr("Error wrong id."));
                    file.close();
                    Clear();
                    return;
                }
                catch (const VExceptionUniqueId &e)
                {
                    e.CriticalMessageBox(tr("Error no unique id."));
                    file.close();
                    Clear();
                    return;
                }
                connect(comboBoxDraws,  static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                        this, &MainWindow::currentDrawChanged);
                QString nameDraw = doc->GetNameActivDraw();
                qint32 index = comboBoxDraws->findText(nameDraw);
                if ( index != -1 )
                { // -1 for not found
                    comboBoxDraws->setCurrentIndex(index);
                }
                if (comboBoxDraws->count() > 0)
                {
                    SetEnableTool(true);
                }
                else
                {
                    SetEnableTool(false);
                }
                SetEnableWidgets(true);
            }
            else
            {
                QMessageBox msgBox;
                msgBox.setWindowTitle(tr("Error!"));
                msgBox.setText(tr("Parsing pattern file error."));
                msgBox.setInformativeText(errorMsg);
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                QString error = QString(tr("Error in line %1 column %2")).arg(errorLine).arg(errorColumn);
                msgBox.setDetailedText(error);
                msgBox.exec();
                file.close();
                Clear();
                return;
            }
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Error!"));
            msgBox.setText(tr("Validation file error."));
            msgBox.setInformativeText(errorMsg);
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            QString error = QString(tr("Error in line %1 column %2")).arg(errorLine).arg(errorColumn);
            msgBox.setDetailedText(error);
            msgBox.exec();
            file.close();
            Clear();
            return;
        }
        file.close();
    }
    else
    {
        qWarning()<<tr("Can't open pattern file.")<<Q_FUNC_INFO;
    }
    this->fileName = fileName;
    QFileInfo info(fileName);
    QString title(info.fileName());
    title.append("-Valentina");
    setWindowTitle(title);
    helpLabel->setText("");
}
