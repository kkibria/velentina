/************************************************************************
 **
 **  @file   vmodelingline.h
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

#ifndef VMODELINGLINE_H
#define VMODELINGLINE_H

#include "vmodelingtool.h"
#include <QGraphicsLineItem>
#include "../../dialogs/dialogline.h"

/**
 * @brief The VModelingLine class
 */
class VModelingLine: public VModelingTool, public QGraphicsLineItem
{
    Q_OBJECT
public:
                          /**
                           * @brief VModelingLine
                           * @param doc dom document container
                           * @param data
                           * @param id
                           * @param firstPoint
                           * @param secondPoint
                           * @param typeCreation
                           * @param parent
                           */
                          VModelingLine(VDomDocument *doc, VContainer *data, qint64 id, qint64 firstPoint,
                                        qint64 secondPoint, const Tool::Sources &typeCreation,
                                        QGraphicsItem * parent = 0);
    /**
     * @brief setDialog
     */
    virtual void          setDialog();
    /**
     * @brief Create
     * @param dialog
     * @param doc dom document container
     * @param data
     * @return
     */
    static VModelingLine* Create(QSharedPointer<DialogLine> &dialog, VDomDocument *doc, VContainer *data);
    /**
     * @brief Create
     * @param _id
     * @param firstPoint
     * @param secondPoint
     * @param doc dom document container
     * @param data
     * @param parse
     * @param typeCreation
     * @return
     */
    static VModelingLine* Create(const qint64 &_id, const qint64 &firstPoint, const qint64 &secondPoint,
                                 VDomDocument *doc, VContainer *data, const Document::Documents &parse,
                                 const Tool::Sources &typeCreation);
    /**
     * @brief TagName
     */
    static const QString  TagName;
    /**
     * @brief ToolType
     */
    static const QString  ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile
     */
    virtual void          FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui
     * @param result
     */
    virtual void          FullUpdateFromGui(int result);
protected:
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void          contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile
     */
    virtual void          AddToFile();
    /**
     * @brief hoverMoveEvent
     * @param event
     */
    virtual void          hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief hoverLeaveEvent
     * @param event
     */
    virtual void          hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    /**
     * @brief RemoveReferens
     */
    virtual void          RemoveReferens();
private:
    /**
     * @brief firstPoint
     */
    qint64                firstPoint;
    /**
     * @brief secondPoint
     */
    qint64                secondPoint;
    /**
     * @brief dialogLine
     */
    QSharedPointer<DialogLine> dialogLine;
};

#endif // VMODELINGLINE_H
