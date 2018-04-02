/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 5, 2017
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2017 Valentina project
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

#ifndef VCURVEPATHITEM_H
#define VCURVEPATHITEM_H

#include <QGraphicsPathItem>
#include <QtGlobal>

#include "../vmisc/def.h"

class VCurvePathItem : public QGraphicsPathItem
{
public:
    explicit VCurvePathItem(QGraphicsItem *parent = nullptr);
    virtual ~VCurvePathItem() = default;

    virtual QPainterPath shape() const Q_DECL_OVERRIDE;

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget *widget = nullptr) Q_DECL_OVERRIDE;

    virtual int  type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Vis::CurvePathItem)};

    void SetDirectionArrows(const QVector<QPair<QLineF, QLineF>> &arrows);
    void SetPoints(const QVector<QPointF> &points);
    void SetWidth(qreal width);
protected:
    virtual void ScalePenWidth();
private:
    Q_DISABLE_COPY(VCurvePathItem)

    QVector<QPair<QLineF, QLineF>> m_directionArrows;
    QVector<QPointF> m_points;
    qreal m_defaultWidth;
};

#endif // VCURVEPATHITEM_H
