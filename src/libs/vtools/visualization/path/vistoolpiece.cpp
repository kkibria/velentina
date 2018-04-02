/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   5 11, 2016
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentina project, a pattern making
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

#include "vistoolpiece.h"
#include "../vpatterndb/vpiecepath.h"
#include "../vgeometry/vpointf.h"
#include "../vwidgets/scalesceneitems.h"

//---------------------------------------------------------------------------------------------------------------------
VisToolPiece::VisToolPiece(const VContainer *data, QGraphicsItem *parent)
    : VisPath(data, parent),
      m_points(),
      m_line1(nullptr),
      m_line2(nullptr),
      m_piece(),
      m_pieceCached(false),
      m_cachedMainPath(),
      m_cachedNodes(),
      m_cachedMainPathPoints()
{
    m_line1 = InitItem<VScaledLine>(supportColor, this);
    m_line2 = InitItem<VScaledLine>(supportColor, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::RefreshGeometry()
{
    HideAllItems();

    if (m_piece.GetPath().CountNodes() > 0)
    {
        if (not m_pieceCached)
        {
            m_cachedMainPath = m_piece.MainPathPath(Visualization::data);
            m_cachedNodes = m_piece.MainPathNodePoints(Visualization::data);
            if (mode == Mode::Creation)
            {
                m_cachedMainPathPoints = m_piece.MainPathPoints(Visualization::data);
            }
            m_pieceCached = true;
        }

        DrawPath(this, m_cachedMainPath, mainColor, Qt::SolidLine, Qt::RoundCap);

        for (int i = 0; i < m_cachedNodes.size(); ++i)
        {
            VScaledEllipse *point = GetPoint(static_cast<quint32>(i), supportColor);
            DrawPoint(point, m_cachedNodes.at(i).toQPointF(), supportColor);
        }

        if (mode == Mode::Creation)
        {
            DrawLine(m_line1, QLineF(m_cachedMainPathPoints.first(), Visualization::scenePos), supportColor,
                     Qt::DashLine);

            if (m_cachedMainPathPoints.size() > 1)
            {
                DrawLine(m_line2, QLineF(m_cachedMainPathPoints.last(), Visualization::scenePos), supportColor,
                         Qt::DashLine);
            }
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::SetPiece(const VPiece &piece)
{
    m_piece = piece;
    m_pieceCached = false;
}

//---------------------------------------------------------------------------------------------------------------------
VScaledEllipse *VisToolPiece::GetPoint(quint32 i, const QColor &color)
{
    return GetPointItem(m_points, i, color, this);
}

//---------------------------------------------------------------------------------------------------------------------
void VisToolPiece::HideAllItems()
{
    if (m_line1)
    {
        m_line1->setVisible(false);
    }

    if (m_line2)
    {
        m_line2->setVisible(false);
    }

    for (int i=0; i < m_points.size(); ++i)
    {
        if (QGraphicsEllipseItem *item = m_points.at(i))
        {
            item->setVisible(false);
        }
    }
}
