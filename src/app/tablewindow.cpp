/************************************************************************
 **
 **  @file   tablewindow.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
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

#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "widgets/vtablegraphicsview.h"
#include "core/vapplication.h"
#include "../../libs/vobj/vobjpaintdevice.h"
#include "../dialogs/app/dialoglayoutsettings.h"
#include "../../libs/vlayout/vlayoutgenerator.h"
#include "../dialogs/app/dialoglayoutprogress.h"

#include <QtSvg>
#include <QPrinter>
#include <QGraphicsScene>
#include <QtCore/qmath.h>

#ifdef Q_OS_WIN
#   define PDFTOPS "pdftops.exe"
#else
#   define PDFTOPS "pdftops"
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief TableWindow constructor.
 * @param parent parent widget.
 */
TableWindow::TableWindow(QWidget *parent)
    :QMainWindow(parent), ui(new Ui::TableWindow),
    listDetails(QVector<VLayoutDetail>()), papers(QList<QGraphicsItem *>()), shadows(QList<QGraphicsItem *>()),
    scenes(QList<QGraphicsScene *>()), fileName(QString()), description(QString()), paperHeight(0), paperWidth(0),
    shift(0), layoutWidth(0), group(Cases::CaseDesc), tempScene(nullptr)
{
    ui->setupUi(this);
    tempScene = new QGraphicsScene(QRectF(0, 0, qApp->toPixel(823, Unit::Mm), qApp->toPixel(1171, Unit::Mm)));
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );
    tempScene->setBackgroundBrush( brush );

    ui->view->setScene(tempScene);
    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
    ui->horizontalLayout->addWidget(ui->view);
    //connect(ui->actionZoomIn, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomIn);
    //connect(ui->actionZoomOut, &QAction::triggered, ui->view, &VTableGraphicsView::ZoomOut);
    connect(ui->actionStop, &QAction::triggered, this, &TableWindow::StopTable);
    //connect(ui->actionSave, &QAction::triggered, this, &TableWindow::saveScene);
    connect(ui->actionLayout, &QAction::triggered, this, &TableWindow::Layout);
    connect(ui->listWidget, &QListWidget::currentRowChanged, this, &TableWindow::ShowPaper);
}

//---------------------------------------------------------------------------------------------------------------------
TableWindow::~TableWindow()
{
    ClearLayout();
    delete ui;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief ModelChosen show window when user want create new layout.
 * @param listDetails list of details.
 * @param description pattern description.
 */
/*
 * Get details for creation layout.
 */
void TableWindow::ModelChosen(QVector<VLayoutDetail> listDetails, const QString &fileName, const QString &description)
{
    this->description = description;

    QString file;
    if (fileName.isEmpty())
    {
        file = tr("untitled");
    }
    else
    {
        file = fileName;
    }
    QFileInfo fi( file );
    this->fileName = fi.baseName();

    this->listDetails = listDetails;
    show();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief closeEvent handle after close window.
 * @param event close event.
 */
void TableWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    StopTable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief moveToCenter move screen to the center of window.
 */
void TableWindow::moveToCenter()
{
    QRect rect = frameGeometry();
    rect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(rect.topLeft());
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief showEvent handle after show window.
 * @param event show event.
 */
void TableWindow::showEvent ( QShowEvent * event )
{
    QMainWindow::showEvent(event);
    moveToCenter();
    ui->view->fitInView(ui->view->scene()->sceneRect(), Qt::KeepAspectRatio);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief StopTable stop creation layout.
 */
void TableWindow::StopTable()
{
    hide();
    ClearLayout();
    emit closed();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief saveScene save created layout.
 */
void TableWindow::saveScene()
{
//    QMap<QString, QString> extByMessage;
//    extByMessage[ tr("Svg files (*.svg)") ] = ".svg";
//    extByMessage[ tr("PDF files (*.pdf)") ] = ".pdf";
//    extByMessage[ tr("Images (*.png)") ] = ".png";
//    extByMessage[ tr("Wavefront OBJ (*.obj)") ] = ".obj";

//    QProcess proc;
//    proc.start(PDFTOPS);
//    if (proc.waitForFinished(15000))
//    {
//        extByMessage[ tr("PS files (*.ps)") ] = ".ps";
//        extByMessage[ tr("EPS files (*.eps)") ] = ".eps";
//    }
//    else
//    {
//        qWarning()<<PDFTOPS<<"error"<<proc.error()<<proc.errorString();
//    }

//    QString saveMessage;
//    QMapIterator<QString, QString> i(extByMessage);
//    while (i.hasNext())
//    {
//        i.next();
//        saveMessage += i.key();
//        if (i.hasNext())
//        {
//            saveMessage += ";;";
//        }
//    }

//    QString sf;
//    // the save function
//    QString dir = QDir::homePath()+"/"+fileName;
//    QString name = QFileDialog::getSaveFileName(this, tr("Save layout"), dir, saveMessage, &sf);

//    if (name.isEmpty())
//    {
//        return;
//    }

//    // what if the user did not specify a suffix...?
//    QString suf = extByMessage.value(sf);
//    suf.replace(".", "");
//    QFileInfo f( name );
//    if (f.suffix().isEmpty() || f.suffix() != suf)
//    {
//        name += extByMessage.value(sf);
//    }

//    QBrush *brush = new QBrush();
//    brush->setColor( QColor( Qt::white ) );
//    tableScene->setBackgroundBrush( *brush );
//    tableScene->clearSelection(); // Selections would also render to the file, so need delete them
//    shadowPaper->setVisible(false);
//    paper->setPen(QPen(Qt::white, 0.1, Qt::NoPen));
//    QFileInfo fi( name );
//    QStringList suffix = QStringList() << "svg" << "png" << "pdf" << "eps" << "ps" << "obj";
//    switch (suffix.indexOf(fi.suffix()))
//    {
//        case 0: //svg
//            paper->setVisible(false);
//            SvgFile(name);
//            paper->setVisible(true);
//            break;
//        case 1: //png
//            PngFile(name);
//            break;
//        case 2: //pdf
//            PdfFile(name);
//            break;
//        case 3: //eps
//            EpsFile(name);
//            break;
//        case 4: //ps
//            PsFile(name);
//            break;
//        case 5: //obj
//            paper->setVisible(false);
//            ObjFile(name);
//            paper->setVisible(true);
//            break;
//        default:
//            qDebug() << "Can't recognize file suffix. File file "<<name<<Q_FUNC_INFO;
//            break;
//    }
//    paper->setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine())));
//    brush->setColor( QColor( Qt::gray ) );
//    brush->setStyle( Qt::SolidPattern );
//    tableScene->setBackgroundBrush( *brush );
//    shadowPaper->setVisible(true);
//    delete brush;
}

void TableWindow::ShowPaper(int index)
{
    if (index < 0 || index > scenes.size())
    {
        ui->view->setScene(tempScene);
    }

    ui->view->setScene(scenes.at(index));
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::Layout()
{
    DialogLayoutSettings layout(this);
    if (paperHeight != 0)
    {
        layout.SetPaperHeight(paperHeight);
        layout.SetPaperWidth(paperWidth);
        layout.SetLayoutWidth(layoutWidth);
        layout.SetShift(shift);
        layout.SetGroup(group);
    }

    if (layout.exec() == QDialog::Rejected)
    {
        return;
    }

    paperHeight = layout.GetPaperHeight();
    paperWidth = layout.GetPaperWidth();
    shift = layout.GetShift();
    layoutWidth = layout.GetLayoutWidth();
    group = layout.GetGroup();

    VLayoutGenerator lGenerator(this);
    lGenerator.SetDetails(listDetails);
    lGenerator.SetLayoutWidth(layoutWidth);
    lGenerator.SetCaseType(group);
    lGenerator.SetPaperHeight(paperHeight);
    lGenerator.SetPaperWidth(paperWidth);
    lGenerator.SetShift(shift);

    DialogLayoutProgress progress(lGenerator.DetailsCount(), this);

    connect(&lGenerator, &VLayoutGenerator::Start, &progress, &DialogLayoutProgress::Start);
    connect(&lGenerator, &VLayoutGenerator::Arranged, &progress, &DialogLayoutProgress::Arranged);
    connect(&lGenerator, &VLayoutGenerator::Error, &progress, &DialogLayoutProgress::Error);
    connect(&lGenerator, &VLayoutGenerator::Finished, &progress, &DialogLayoutProgress::Finished);
    connect(&progress, &DialogLayoutProgress::Abort, &lGenerator, &VLayoutGenerator::Abort);

    lGenerator.Generate();

    if (lGenerator.State() == LayoutErrors::NoError)
    {
        ClearLayout();
        papers = lGenerator.GetItems();
        CreateShadows();
        CreateScenes();
        // Create previews
        PrepareSceneList();
    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief SvgFile save layout to svg file.
 * @param name name layout file.
 */
void TableWindow::SvgFile(const QString &name) const
{
//    QSvgGenerator generator;
//    generator.setFileName(name);
//    generator.setSize(paper->rect().size().toSize());
//    generator.setViewBox(paper->rect());
//    generator.setTitle("Valentina pattern");
//    generator.setDescription(description);
//    generator.setResolution(static_cast<int>(qApp->PrintDPI));
//    QPainter painter;
//    painter.begin(&generator);
//    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthHairLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.setBrush ( QBrush ( Qt::NoBrush ) );
//    tableScene->render(&painter);
//    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PngFile save layout to png file.
 * @param name name layout file.
 */
void TableWindow::PngFile(const QString &name) const
{
//    QRectF r = paper->rect();
//    qreal x=0, y=0, w=0, h=0;
//    r.getRect(&x, &y, &w, &h);// Re-shrink the scene to it's bounding contents
//    // Create the image with the exact size of the shrunk scene
//    QImage image(QSize(static_cast<qint32>(w), static_cast<qint32>(h)), QImage::Format_ARGB32);
//    image.fill(Qt::transparent);                                              // Start all pixels transparent
//    QPainter painter(&image);
//    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.setBrush ( QBrush ( Qt::NoBrush ) );
//    tableScene->render(&painter);
//    image.save(name);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfFile save layout to pdf file.
 * @param name name layout file.
 */
void TableWindow::PdfFile(const QString &name) const
{
//    QPrinter printer;
//    printer.setOutputFormat(QPrinter::PdfFormat);
//    printer.setOutputFileName(name);
//    QRectF r = paper->rect();
//    qreal x=0, y=0, w=0, h=0;
//    r.getRect(&x, &y, &w, &h);// Re-shrink the scene to it's bounding contents
//    printer.setResolution(static_cast<int>(qApp->PrintDPI));
//    printer.setPaperSize ( QSizeF(qApp->fromPixel(w, Unit::Mm), qApp->fromPixel(h, Unit::Mm)), QPrinter::Millimeter );
//    QPainter painter;
//    if (painter.begin( &printer ) == false)
//    { // failed to open file
//        qCritical("Can't open printer %s", qPrintable(name));
//        return;
//    }
//    painter.setFont( QFont( "Arial", 8, QFont::Normal ) );
//    painter.setRenderHint(QPainter::Antialiasing, true);
//    painter.setPen(QPen(Qt::black, qApp->toPixel(qApp->widthMainLine()), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
//    painter.setBrush ( QBrush ( Qt::NoBrush ) );
//    tableScene->render(&painter);
//    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief EpsFile save layout to eps file.
 * @param name name layout file.
 */
void TableWindow::EpsFile(const QString &name) const
{
//    QTemporaryFile tmp;
//    if (tmp.open())
//    {
//        PdfFile(tmp.fileName());
//        QStringList params = QStringList() << "-eps" << tmp.fileName() << name;
//        PdfToPs(params);
//    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PsFile save layout to ps file.
 * @param name name layout file.
 */
void TableWindow::PsFile(const QString &name) const
{
//    QTemporaryFile tmp;
//    if (tmp.open())
//    {
//        PdfFile(tmp.fileName());
//        QStringList params = QStringList() << tmp.fileName() << name;
//        PdfToPs(params);
//    }
}

//---------------------------------------------------------------------------------------------------------------------
/**
 * @brief PdfToPs use external tool "pdftops" for converting pdf too eps or ps format.
 * @param params string with parameter for tool. Parameters have format: "-eps input_file out_file". Use -eps when
 * need create eps file.
 */
void TableWindow::PdfToPs(const QStringList &params) const
{
//#ifndef QT_NO_CURSOR
//    QApplication::setOverrideCursor(Qt::WaitCursor);
//#endif
//    QProcess proc;
//    proc.start(PDFTOPS, params);
//    proc.waitForFinished(15000);
//#ifndef QT_NO_CURSOR
//    QApplication::restoreOverrideCursor();
//#endif

//    QFile f(params.last());
//    if (f.exists() == false)
//    {
//        QString msg = QString(tr("Creating file '%1' failed! %2")).arg(params.last()).arg(proc.errorString());
//        QMessageBox msgBox(QMessageBox::Critical, tr("Critical error!"), msg, QMessageBox::Ok | QMessageBox::Default);
//        msgBox.exec();
//    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::ObjFile(const QString &name) const
{
//    VObjPaintDevice generator;
//    generator.setFileName(name);
//    generator.setSize(paper->rect().size().toSize());
//    generator.setResolution(static_cast<int>(qApp->PrintDPI));
//    QPainter painter;
//    painter.begin(&generator);
//    tableScene->render(&painter);
//    painter.end();
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::ClearLayout()
{
    qDeleteAll (scenes);
    scenes.clear();
    listDetails.clear();
    shadows.clear();
    papers.clear();
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::CreateShadows()
{
    for (int i=0; i< papers.size(); ++i)
    {
        qreal x1=0, y1=0, x2=0, y2=0;
        QGraphicsRectItem *item = qgraphicsitem_cast<QGraphicsRectItem *>(papers.at(i));
        if (item)
        {
            item->rect().getCoords(&x1, &y1, &x2, &y2);
            QGraphicsRectItem *shadowPaper = new QGraphicsRectItem(QRectF(x1+4, y1+4, x2+4, y2+4));
            shadowPaper->setBrush(QBrush(Qt::black));
            shadows.append(shadowPaper);
        }
        else
        {
            shadows.append(nullptr);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::CreateScenes()
{
    QBrush brush;
    brush.setStyle( Qt::SolidPattern );
    brush.setColor( QColor( Qt::gray ) );

    for (int i=0; i<papers.size(); ++i)
    {
        QGraphicsScene *scene = new QGraphicsScene();
        scene->setBackgroundBrush(brush);
        scene->addItem(shadows.at(i));
        scene->addItem(papers.at(i));
        scenes.append(scene);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TableWindow::PrepareSceneList()
{
    const QIcon ico("://icon/64x64/icon64x64.png");
    for (int i=0; i<scenes.size(); ++i)
    {
        QListWidgetItem *item = new QListWidgetItem(ico, QString::number(i));
        ui->listWidget->addItem(item);
    }
}
