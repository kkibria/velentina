/************************************************************************
 **
 **  @file   abstracttest.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   7 5, 2015
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2015 Valentina project
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

#include "abstracttest.h"

#include <QtTest>

//---------------------------------------------------------------------------------------------------------------------
AbstractTest::AbstractTest(QObject *parent) :
    QObject(parent)
{
}

//---------------------------------------------------------------------------------------------------------------------
void AbstractTest::Comparison(const QVector<QPointF> &ekv, const QVector<QPointF> &ekvOrig) const
{
    // Begin comparison
    QCOMPARE(ekv.size(), ekvOrig.size());// First check if sizes equal
    for (int i=0; i < ekv.size(); i++)
    {
        // Check each point
        QCOMPARE(ekv.at(i).toPoint(), ekvOrig.at(i).toPoint()); // Don't use comparison float values
    }
}