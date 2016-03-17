/************************************************************************
 **
 **  @file   vcubicbezierpath.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   16 3, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2016 Valentina project
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

#include "vcubicbezierpath.h"
#include "vcubicbezierpath_p.h"
#include "vspline.h"
#include "../ifc/exception/vexception.h"

#if QT_VERSION < QT_VERSION_CHECK(5, 1, 0)
#   include "../vmisc/vmath.h"
#else
#   include <QtMath>
#endif

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(quint32 idObject, Draw mode)
    : VAbstractCubicBezierPath(GOType::SplinePath, idObject, mode),
      d(new VCubicBezierPathData())
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(const VCubicBezierPath &curve)
    : VAbstractCubicBezierPath(curve),
      d(curve.d)
{
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::VCubicBezierPath(const QVector<VPointF> &points, quint32 idObject, Draw mode)
    : VAbstractCubicBezierPath(GOType::SplinePath, idObject, mode),
      d(new VCubicBezierPathData())
{
    if (points.isEmpty())
    {
        return;
    }

    d->path = points;
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath &VCubicBezierPath::operator=(const VCubicBezierPath &curve)
{
    if ( &curve == this )
    {
        return *this;
    }
    VAbstractCurve::operator=(curve);
    d = curve.d;
    return *this;
}

//---------------------------------------------------------------------------------------------------------------------
VCubicBezierPath::~VCubicBezierPath()
{
}

//---------------------------------------------------------------------------------------------------------------------
VPointF &VCubicBezierPath::operator[](int indx)
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
const VPointF &VCubicBezierPath::at(int indx) const
{
    return d->path[indx];
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezierPath::append(const VPointF &point)
{
    if (d->path.size() > 0 && d->path.last().toQPointF() != point.toQPointF())
    {
        return;
    }

    d->path.append(point);
    CreateName();
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::CountSubSpl() const
{
    return qFloor(qAbs((d->path.size() - 4) / 3 + 1));
}

//---------------------------------------------------------------------------------------------------------------------
qint32 VCubicBezierPath::CountPoints() const
{
    return d->path.size();
}

//---------------------------------------------------------------------------------------------------------------------
void VCubicBezierPath::Clear()
{
    d->path.clear();
    SetDuplicate(0);
}

//---------------------------------------------------------------------------------------------------------------------
VSpline VCubicBezierPath::GetSpline(qint32 index) const
{
    if (CountPoints() < 4)
    {
        throw VException(tr("Not enough points to create the spline."));
    }

    if (index < 1 || index > CountSubSpl())
    {
        throw VException(tr("This spline does not exist."));
    }

    const qint32 base = (index - 1) * 3;

    // Correction the first control point of each next spline curve except for the first.
    QPointF p2 = d->path.at(base + 1).toQPointF();
    if (base + 1 > 1)
    {
        const QPointF b = d->path.at(base).toQPointF();
        QLineF foot1(b, d->path.at(base - 1).toQPointF());
        QLineF foot2(b, p2);

        foot2.setAngle(foot1.angle() + 180);
        p2 = foot2.p2();
    }

    VSpline spl(d->path.at(base), p2, d->path.at(base + 2).toQPointF(), d->path.at(base + 3));
    return spl;
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezierPath::GetStartAngle() const
{
    if (CountSubSpl() > 0)
    {
        return GetSpline(1).GetStartAngle();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VCubicBezierPath::GetEndAngle() const
{
    const qint32 count = CountSubSpl();
    if (count > 0)
    {
        return GetSpline(count).GetEndAngle();
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QVector<VPointF> VCubicBezierPath::GetSplinePath() const
{
    return d->path;
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezierPath::FirstPoint() const
{
    if (not d->path.isEmpty())
    {
        return d->path.first();
    }
    else
    {
        return VPointF();
    }
}

//---------------------------------------------------------------------------------------------------------------------
VPointF VCubicBezierPath::LastPoint() const
{
    const qint32 count = CountSubSpl();
    if (count >= 1)
    {
        const qint32 base = (count - 1) * 3;
        return d->path.at(base + 3);// Take last point of the last real spline
    }
    else
    {
        return VPointF();
    }
}