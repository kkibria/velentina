/************************************************************************
 **
 **  @file
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   12 9, 2016
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

#ifndef VTOOLFLIPPINGBYLINE_H
#define VTOOLFLIPPINGBYLINE_H

#include <QtGlobal>

#include "vabstractflipping.h"

struct VToolFlippingByLineInitData : VAbstractOperationInitData
{
    VToolFlippingByLineInitData()
        : VAbstractOperationInitData(),
          firstLinePointId(NULL_ID),
          secondLinePointId(NULL_ID)
    {}

    quint32 firstLinePointId;
    quint32 secondLinePointId;
};


class VToolFlippingByLine : public VAbstractFlipping
{
    Q_OBJECT
public:
    virtual ~VToolFlippingByLine() Q_DECL_EQ_DEFAULT;
    virtual void setDialog() Q_DECL_OVERRIDE;
    static VToolFlippingByLine* Create(QSharedPointer<DialogTool> dialog, VMainGraphicsScene *scene,
                                       VAbstractPattern *doc, VContainer *data);
    static VToolFlippingByLine* Create(VToolFlippingByLineInitData initData);

    static const QString ToolType;

    virtual int type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::FlippingByLine)};

    QString FirstLinePointName() const;
    QString SecondLinePointName() const;

    virtual void ShowVisualization(bool show) Q_DECL_OVERRIDE;
protected slots:
    virtual void ShowContextMenu(QGraphicsSceneContextMenuEvent *event, quint32 id=NULL_ID) Q_DECL_OVERRIDE;
protected:
    virtual void    SetVisualization() Q_DECL_OVERRIDE;
    virtual void    SaveDialog(QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void    ReadToolAttributes(const QDomElement &domElement) Q_DECL_OVERRIDE;
    virtual void    SaveOptions(QDomElement &tag, QSharedPointer<VGObject> &obj) Q_DECL_OVERRIDE;
    virtual QString MakeToolTip() const Q_DECL_OVERRIDE;
private:
    Q_DISABLE_COPY(VToolFlippingByLine)

    quint32 m_firstLinePointId;
    quint32 m_secondLinePointId;

    VToolFlippingByLine(const VToolFlippingByLineInitData &initData, QGraphicsItem *parent = nullptr);
};

#endif // VTOOLFLIPPINGBYLINE_H
