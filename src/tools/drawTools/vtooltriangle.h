/************************************************************************
 **
 **  @file   vtooltriangle.h
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

#ifndef VTOOLTRIANGLE_H
#define VTOOLTRIANGLE_H

#include "vtoolpoint.h"
#include "../../dialogs/dialogtriangle.h"

/**
 * @brief The VToolTriangle class
 */
class VToolTriangle : public VToolPoint
{
    Q_OBJECT
public:
                   /**
                    * @brief VToolTriangle
                    * @param doc dom document container
                    * @param data container with variables
                    * @param id object id in container
                    * @param axisP1Id
                    * @param axisP2Id
                    * @param firstPointId
                    * @param secondPointId
                    * @param typeCreation way we create this tool.
                    * @param parent parent object
                    */
                   VToolTriangle(VDomDocument *doc, VContainer *data, const qint64 &id, const qint64 &axisP1Id,
                                 const qint64 &axisP2Id, const qint64 &firstPointId, const qint64 &secondPointId,
                                 const Tool::Sources &typeCreation, QGraphicsItem * parent = 0);
    /**
     * @brief setDialog set dialog when user want change tool option.
     */
    virtual void   setDialog();
    /**
     * @brief Create help create tool
     * @param dialog
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     */
    static void    Create(QSharedPointer<DialogTriangle> &dialog, VMainGraphicsScene  *scene, VDomDocument *doc,
                          VContainer *data);
    /**
     * @brief Create help create tool
     * @param _id tool id, 0 if tool doesn't exist yet.
     * @param pointName
     * @param axisP1Id
     * @param axisP2Id
     * @param firstPointId
     * @param secondPointId
     * @param mx label bias x axis.
     * @param my label bias y axis.
     * @param scene pointer to scene.
     * @param doc dom document container
     * @param data container with variables
     * @param parse parser file mode.
     * @param typeCreation way we create this tool.
     */
    static void    Create(const qint64 _id, const QString &pointName, const qint64 &axisP1Id, const qint64 &axisP2Id,
                          const qint64 &firstPointId, const qint64 &secondPointId, const qreal &mx, const qreal &my,
                          VMainGraphicsScene *scene, VDomDocument *doc, VContainer *data,
                          const Document::Documents &parse, const Tool::Sources &typeCreation);
    /**
     * @brief FindPoint
     * @param axisP1
     * @param axisP2
     * @param firstPoint
     * @param secondPoint
     * @return
     */
    static QPointF FindPoint(const QPointF &axisP1, const QPointF &axisP2, const QPointF &firstPoint,
                             const QPointF &secondPoint);
    /**
     * @brief ToolType
     */
    static const QString ToolType;
public slots:
    /**
     * @brief FullUpdateFromFile update tool data form file.
     */
    virtual void   FullUpdateFromFile();
    /**
     * @brief FullUpdateFromGui  refresh tool data from change options.
     * @param result
     */
    virtual void   FullUpdateFromGui(int result);
    virtual void   ShowContextMenu(QGraphicsSceneContextMenuEvent *event);
protected:
    /**
     * @brief RemoveReferens decrement value of reference.
     */
    virtual void   RemoveReferens();
    /**
     * @brief contextMenuEvent
     * @param event
     */
    virtual void   contextMenuEvent ( QGraphicsSceneContextMenuEvent * event );
    /**
     * @brief AddToFile add tag with informations about tool into file.
     */
    virtual void   AddToFile();
    /**
     * @brief RefreshDataInFile refresh attributes in file. If attributes don't exist create them.
     */
    virtual void RefreshDataInFile();
private:
    Q_DISABLE_COPY(VToolTriangle)
    /**
     * @brief axisP1Id
     */
    qint64         axisP1Id;
    /**
     * @brief axisP2Id
     */
    qint64         axisP2Id;
    /**
     * @brief firstPointId
     */
    qint64         firstPointId;
    /**
     * @brief secondPointId
     */
    qint64         secondPointId;
    /**
     * @brief dialogTriangle
     */
    QSharedPointer<DialogTriangle> dialogTriangle;
};

#endif // VTOOLTRIANGLE_H
