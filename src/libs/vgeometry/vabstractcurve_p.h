/************************************************************************
 **
 **  @file   vabstractcurve_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 2, 2016
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

#ifndef VABSTRACTCURVE_P_H
#define VABSTRACTCURVE_P_H

#include <QSharedData>

#ifdef Q_CC_GNU
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Weffc++"
#endif

class VAbstractCurveData : public QSharedData
{
public:

    VAbstractCurveData ()
        : duplicate(0)
    {}

    VAbstractCurveData(const VAbstractCurveData &curve)
        : QSharedData(curve), duplicate(curve.duplicate)
    {}

    virtual ~VAbstractCurveData();

    /** @brief duplicate helps create unique name for curves that connects the same start and finish points. */
    quint32 duplicate;

private:
    VAbstractCurveData &operator=(const VAbstractCurveData &) Q_DECL_EQ_DELETE;
};

VAbstractCurveData::~VAbstractCurveData()
{}

#ifdef Q_CC_GNU
#pragma GCC diagnostic pop
#endif

#endif // VABSTRACTCURVE_P_H