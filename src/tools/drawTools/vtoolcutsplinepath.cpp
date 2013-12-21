/************************************************************************
 **
 **  @file   vtoolcutsplinepath.cpp
 **  @author Roman Telezhinsky <dismine@gmail.com>
 **  @date   15 12, 2013
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

#include "vtoolcutsplinepath.h"
#include "../../container/calculator.h"

const QString VToolCutSplinePath::ToolType = QStringLiteral("cutSplinePath");
const QString VToolCutSplinePath::AttrSplinePath = QStringLiteral("splinePath");

VToolCutSplinePath::VToolCutSplinePath(VDomDocument *doc, VContainer *data, const qint64 &id,
                                       const QString &formula, const qint64 &splinePathId,
                                       const qint64 &splPath1id, const qint64 &splPath2id,
                                       const Tool::Sources &typeCreation, QGraphicsItem *parent)
    :VToolPoint(doc, data, id, parent), formula(formula), splinePathId(splinePathId),
      dialogCutSplinePath(QSharedPointer<DialogCutSplinePath>()), firstSpline(), secondSpline(),
      splPath1id (splPath1id), splPath2id(splPath2id)
{
    Q_ASSERT_X(splinePathId > 0, Q_FUNC_INFO, "splinePathId <= 0");
    Q_ASSERT_X(splPath1id > 0, Q_FUNC_INFO, "spl1id <= 0");
    Q_ASSERT_X(splPath2id > 0, Q_FUNC_INFO, "spl2id <= 0");

    firstSpline = new VSimpleSpline(splPath1id, &currentColor, &factor);
    Q_ASSERT(firstSpline != 0);
    RefreshSpline(firstSpline, splPath1id, SimpleSpline::ForthPoint);
    firstSpline->setParentItem(this);
    connect(firstSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

    secondSpline = new VSimpleSpline(splPath2id, &currentColor, &factor);
    Q_ASSERT(secondSpline != 0);
    RefreshSpline(secondSpline, splPath2id, SimpleSpline::FirstPoint);
    secondSpline->setParentItem(this);
    connect(secondSpline, &VSimpleSpline::Choosed, this, &VToolCutSplinePath::SplineChoosed);

    if (typeCreation == Tool::FromGui)
    {
        AddToFile();
    }
}

void VToolCutSplinePath::setDialog()
{
    Q_ASSERT(dialogCutSplinePath.isNull() == false);
    VPointF point = VAbstractTool::data.GetPoint(id);
    dialogCutSplinePath->setFormula(formula);
    dialogCutSplinePath->setSplinePathId(splinePathId, id);
    dialogCutSplinePath->setPointName(point.name());
}

void VToolCutSplinePath::Create(QSharedPointer<DialogCutSplinePath> &dialog, VMainGraphicsScene *scene,
                            VDomDocument *doc, VContainer *data)
{
    QString pointName = dialog->getPointName();
    QString formula = dialog->getFormula();
    qint64 splinePathId = dialog->getSplinePathId();
    Create(0, pointName, formula, splinePathId, 5, 10, scene, doc, data, Document::FullParse, Tool::FromGui);
}

void VToolCutSplinePath::Create(const qint64 _id, const QString &pointName, const QString &formula,
                            const qint64 &splinePathId, const qreal &mx, const qreal &my,
                            VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                            const Document::Documents &parse, const Tool::Sources &typeCreation)
{
    VSplinePath splPath = data->GetSplinePath(splinePathId);
    Calculator cal(data);
    QString errorMsg;
    qreal result = cal.eval(formula, &errorMsg);
    if (errorMsg.isEmpty())
    {
        QPointF spl1p2, spl1p3, spl2p2, spl2p3;
        qint32 p1 = 0, p2 = 0;
        QPointF point = splPath.CutSplinePath(toPixel(result), p1, p2, spl1p2, spl1p3, spl2p2, spl2p3);

        qint64 id = _id;
        qint64 splPath1id = 0;
        qint64 splPath2id = 0;
        if (typeCreation == Tool::FromGui)
        {
            id = data->AddPoint(VPointF(point.x(), point.y(), pointName, mx, my));
            splPath1id = id + 1;
            splPath2id = id + 2;

            VSplinePoint splP1 = splPath[p1];
            VSplinePoint splP2 = splPath[p2];
            VSpline spl1 = VSpline(data->DataPoints(), splP1.P(), spl1p2, spl1p3, id, splPath.getKCurve());
            VSpline spl2 = VSpline(data->DataPoints(), id, spl2p2, spl2p3, splP2.P(), splPath.getKCurve());


            VSplinePath splPath1, splPath2;
            splPath1.setPoints(data->DataPoints());
            splPath2.setPoints(data->DataPoints());
            for(qint32 i = 0; i < splPath.CountPoint(); i++)
            {
                if(i <= p1 && i < p2){
                    if(i == p1)
                    {
                        splPath1.append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1(), spl1.GetKasm1()));
                        VSplinePoint cutPoint = VSplinePoint(id, spl1.GetKasm2(), spl1.GetAngle2()+180, spl2.GetKasm1());
                        splPath1.append(cutPoint);
                        continue;
                    }
                    splPath1.append(splPath[i]);
                }
                else
                {
                    if(i == p2)
                    {
                        VSplinePoint cutPoint = VSplinePoint(id, spl1.GetKasm2(), spl2.GetAngle1(), spl2.GetKasm1());
                        splPath2.append(cutPoint);
                        splPath2.append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2()+180, splP2.KAsm2()));
                        continue;
                    }
                    splPath2.append(splPath[i]);
                }
            }

            splPath1id = data->AddSplinePath(splPath1);
            data->AddLengthSpline(splPath1.name(), toMM(splPath1.GetLength()));

            splPath2id = data->AddSplinePath(splPath2);
            data->AddLengthSpline(splPath2.name(), toMM(splPath2.GetLength()));
        }
        else
        {
            data->UpdatePoint(id, VPointF(point.x(), point.y(), pointName, mx, my));

            splPath1id = id + 1;
            splPath2id = id + 2;

            VSplinePoint splP1 = splPath[p1];
            VSplinePoint splP2 = splPath[p2];
            VSpline spl1 = VSpline(data->DataPoints(), splP1.P(), spl1p2, spl1p3, id, splPath.getKCurve());
            VSpline spl2 = VSpline(data->DataPoints(), id, spl2p2, spl2p3, splP2.P(), splPath.getKCurve());

            VSplinePath splPath1, splPath2;
            splPath1.setPoints(data->DataPoints());
            splPath2.setPoints(data->DataPoints());
            for(qint32 i = 0; i < splPath.CountPoint(); i++)
            {
                if(i <= p1 && i < p2){
                    if(i == p1)
                    {
                        splPath1.append(VSplinePoint(splP1.P(), splP1.KAsm1(), spl1.GetAngle1(), spl1.GetKasm1()));
                        VSplinePoint cutPoint = VSplinePoint(id, spl1.GetKasm2(), spl1.GetAngle2()+180, spl2.GetKasm1());
                        splPath1.append(cutPoint);
                        continue;
                    }
                    splPath1.append(splPath[i]);
                }
                else
                {
                    if(i == p2)
                    {
                        VSplinePoint cutPoint = VSplinePoint(id, spl1.GetKasm2(), spl2.GetAngle1(), spl2.GetKasm1());
                        splPath2.append(cutPoint);
                        splPath2.append(VSplinePoint(splP2.P(), spl2.GetKasm2(), spl2.GetAngle2()+180, splP2.KAsm2()));
                        continue;
                    }
                    splPath2.append(splPath[i]);
                }
            }

            data->UpdateSplinePath(splPath1id, splPath1);
            data->AddLengthSpline(splPath1.name(), toMM(splPath1.GetLength()));

            data->UpdateSplinePath(splPath2id, splPath2);
            data->AddLengthSpline(splPath2.name(), toMM(splPath2.GetLength()));

            if (parse != Document::FullParse)
            {
                doc->UpdateToolData(id, data);
            }
        }
        //VDrawTool::AddRecord(id, Tool::CutSplineTool, doc);
        if (parse == Document::FullParse)
        {
            VToolCutSplinePath *point = new VToolCutSplinePath(doc, data, id, formula, splinePathId, splPath1id,
                                                               splPath2id, typeCreation);
            scene->addItem(point);
            connect(point, &VToolPoint::ChoosedTool, scene, &VMainGraphicsScene::ChoosedItem);
            connect(point, &VToolPoint::RemoveTool, scene, &VMainGraphicsScene::RemoveTool);
            connect(scene, &VMainGraphicsScene::NewFactor, point, &VToolPoint::SetFactor);
            doc->AddTool(id, point);
            doc->AddTool(splPath1id, point);
            doc->AddTool(splPath2id, point);
            doc->IncrementReferens(splinePathId);
        }
    }
}

void VToolCutSplinePath::FullUpdateFromFile()
{
    QDomElement domElement = doc->elementById(QString().setNum(id));
    if (domElement.isElement())
    {
        formula = domElement.attribute(AttrLength, "");
        splinePathId = domElement.attribute(AttrSplinePath, "").toLongLong();
    }
    RefreshGeometry();
}

void VToolCutSplinePath::FullUpdateFromGui(int result)
{
    if (result == QDialog::Accepted)
    {
        QDomElement domElement = doc->elementById(QString().setNum(id));
        if (domElement.isElement())
        {
            domElement.setAttribute(AttrName, dialogCutSplinePath->getPointName());
            domElement.setAttribute(AttrLength, dialogCutSplinePath->getFormula());
            domElement.setAttribute(AttrSplinePath, QString().setNum(dialogCutSplinePath->getSplinePathId()));
            emit FullUpdateTree();
        }
    }
    dialogCutSplinePath.clear();
}

void VToolCutSplinePath::SplineChoosed(qint64 id)
{
    emit ChoosedTool(id, Scene::SplinePath);
}

void VToolCutSplinePath::ChangedActivDraw(const QString &newName)
{
    if (nameActivDraw == newName)
    {
        currentColor = Qt::black;
        firstSpline->setFlag(QGraphicsItem::ItemIsSelectable, true);
        firstSpline->setAcceptHoverEvents(true);
        secondSpline->setFlag(QGraphicsItem::ItemIsSelectable, true);
        secondSpline->setAcceptHoverEvents(true);
    }
    else
    {
        currentColor = Qt::gray;
        firstSpline->setFlag(QGraphicsItem::ItemIsSelectable, false);
        firstSpline->setAcceptHoverEvents(false);
        secondSpline->setFlag(QGraphicsItem::ItemIsSelectable, false);
        secondSpline->setAcceptHoverEvents(false);
    }
    firstSpline->setPen(QPen(currentColor, widthHairLine/factor));
    secondSpline->setPen(QPen(currentColor, widthHairLine/factor));
    VToolPoint::ChangedActivDraw(newName);
}

void VToolCutSplinePath::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    ContextMenu(dialogCutSplinePath, this, event);
}

void VToolCutSplinePath::AddToFile()
{
    VPointF point = VAbstractTool::data.GetPoint(id);
    QDomElement domElement = doc->createElement(TagName);

    AddAttribute(domElement, AttrId, id);
    AddAttribute(domElement, AttrType, ToolType);
    AddAttribute(domElement, AttrName, point.name());
    AddAttribute(domElement, AttrMx, toMM(point.mx()));
    AddAttribute(domElement, AttrMy, toMM(point.my()));

    AddAttribute(domElement, AttrLength, formula);
    AddAttribute(domElement, AttrSplinePath, splinePathId);

    AddToCalculation(domElement);
}

void VToolCutSplinePath::RefreshGeometry()
{
    RefreshSpline(firstSpline, splPath1id, SimpleSpline::ForthPoint);
    RefreshSpline(secondSpline, splPath2id, SimpleSpline::FirstPoint);
    VToolPoint::RefreshPointGeometry(VDrawTool::data.GetPoint(id));
}

void VToolCutSplinePath::RefreshSpline(VSimpleSpline *spline, qint64 splPathid, SimpleSpline::Translation tr)
{
    VSplinePath splPath = VAbstractTool::data.GetSplinePath(splPathid);
    QPainterPath path;
    path.addPath(splPath.GetPath());
    path.setFillRule( Qt::WindingFill );
    if(tr == SimpleSpline::FirstPoint)
    {
        VSpline spl = splPath.GetSpline(1);
        path.translate(-spl.GetPointP1().toQPointF().x(), -spl.GetPointP1().toQPointF().y());
    }
    else
    {
        VSpline spl = splPath.GetSpline(splPath.Count());
        path.translate(-spl.GetPointP4().toQPointF().x(), -spl.GetPointP4().toQPointF().y());
    }
    spline->setPath(path);
}
