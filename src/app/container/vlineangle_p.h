/************************************************************************
 **
 **  @file   vlineangle_p.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   20 8, 2014
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2014 Valentina project
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

#ifndef VLINEANGLE_P_H
#define VLINEANGLE_P_H

#include <QSharedData>
#include "../options.h"

class VLineAngleData : public QSharedData
{
public:

    VLineAngleData()
        :p1Id(NULL_ID), p2Id(NULL_ID)
    {}

    VLineAngleData(const quint32 &p1Id, const quint32 &p2Id)
        :p1Id(p1Id), p2Id(p2Id)
    {}

    VLineAngleData(const VLineAngleData &var)
        :QSharedData(var), p1Id(var.p1Id), p2Id(var.p2Id)
    {}

    virtual  ~VLineAngleData() {}

    quint32 p1Id;
    quint32 p2Id;
};


#endif // VLINEANGLE_P_H
