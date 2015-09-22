/************************************************************************
 **
 **  @file   vtooldetail.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
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

#ifndef VTOOLDETAIL_H
#define VTOOLDETAIL_H

#include "vabstracttool.h"
#include <QGraphicsPathItem>

class VMainGraphicsScene;
class DialogTool;

/**
 * @brief The VToolDetail class for working with detail.
 */
class VToolDetail: public VAbstractTool, public QGraphicsPathItem
{
    Q_OBJECT
public:
    VToolDetail(VAbstractPattern *doc, VContainer *data, const quint32 &id, const Source &typeCreation,
                VMainGraphicsScene *scene, QGraphicsItem * parent = nullptr);
    ~VToolDetail();

    virtual void       setDialog();
    template<typename T>
    /**
     * @brief CreateNode create new node for detail.
     * @param data container.
     * @param id id parent object.
     * @return id for new object.
     */
    static quint32 CreateNode(VContainer *data, const quint32 &id)
    {
        //We can't use exist object. Need create new.
        T *node = new T(*data->GeometricObject<T>(id).data());
        node->setMode(Draw::Modeling);
        return data->AddGObject(node);
    }

    static void        Create(DialogTool *dialog, VMainGraphicsScene *scene, VAbstractPattern *doc, VContainer *data);
    static void        Create(const quint32 &_id, const VDetail &newDetail, VMainGraphicsScene  *scene,
                              VAbstractPattern *doc, VContainer *data, const Document &parse,
                              const Source &typeCreation);
    static const QString TagName;
    static const QString TagNode;
    static const QString AttrSupplement;
    static const QString AttrClosed;
    static const QString AttrWidth;
    static const QString AttrIdObject;
    static const QString AttrNodeType;
    static const QString AttrReverse;
    static const QString NodeTypeContour;
    static const QString NodeTypeModeling;
    static const QString NodeArc;
    static const QString NodePoint;
    static const QString NodeSpline;
    static const QString NodeSplinePath;
    void               Remove(bool ask);
    static void        AddNode(VAbstractPattern *doc, QDomElement &domElement, const VNodeDetail &node);
    virtual int        type() const Q_DECL_OVERRIDE {return Type;}
    enum { Type = UserType + static_cast<int>(Tool::Detail)};
    virtual QString    getTagName() const Q_DECL_OVERRIDE;
    virtual void       ShowVisualization(bool show) Q_DECL_OVERRIDE;
public slots:
    virtual void       FullUpdateFromFile () Q_DECL_OVERRIDE;
    virtual void       FullUpdateFromGuiOk(int result);
protected:
    virtual void       AddToFile () Q_DECL_OVERRIDE;
    virtual void       RefreshDataInFile() Q_DECL_OVERRIDE;
    virtual QVariant   itemChange ( GraphicsItemChange change, const QVariant &value ) Q_DECL_OVERRIDE;
    virtual void       mouseReleaseEvent ( QGraphicsSceneMouseEvent * event ) Q_DECL_OVERRIDE;
    virtual void       contextMenuEvent ( QGraphicsSceneContextMenuEvent * event ) Q_DECL_OVERRIDE;
    virtual void       RemoveReferens() Q_DECL_OVERRIDE;
    virtual void       keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
    virtual void       SetVisualization() Q_DECL_OVERRIDE {}
private:
    Q_DISABLE_COPY(VToolDetail)
    /** @brief dialog dialog options. */
    DialogTool         *dialog;

    /** @brief sceneDetails pointer to the scene. */
    VMainGraphicsScene *sceneDetails;

    void               RefreshGeometry ();
    template <typename Tool>
    /**
     * @brief InitTool initial node item on scene
     * @param scene pointer to scene.
     * @param node node of detail.
     */
    void               InitTool(VMainGraphicsScene *scene, const VNodeDetail &node);
    virtual void       DeleteTool(bool ask = true) Q_DECL_OVERRIDE;
};

#endif // VTOOLDETAIL_H