/************************************************************************
 **
 **  @file   dialogcutsplinrpath.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   15 12, 2013
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

#ifndef DIALOGCUTSPLINEPATH_H
#define DIALOGCUTSPLINEPATH_H

#include "dialogtool.h"

namespace Ui
{
    class DialogCutSplinePath;
}

/**
 * @brief The DialogCutSplinePath class dialog for ToolCutSplinePath.
 */
class DialogCutSplinePath : public DialogTool
{
    Q_OBJECT
public:
    /**
     * @brief DialogCutSplinePath create dialog.
     * @param data container with data
     * @param parent parent widget
     */
    DialogCutSplinePath(const VContainer *data, QWidget *parent = nullptr);
    ~DialogCutSplinePath();
    /**
     * @brief getPointName return name of point
     * @return name
     */
    QString      getPointName() const;
    /**
     * @brief setPointName set name of point
     * @param value name
     */
    void         setPointName(const QString &value);
    /**
     * @brief getFormula return string of formula
     * @return formula
     */
    QString      getFormula() const;
    /**
     * @brief setFormula set string of formula
     * @param value formula
     */
    void         setFormula(const QString &value);
    /**
     * @brief getSplineId return id base point of line
     * @return id
     */
    quint32      getSplinePathId() const;
    /**
     * @brief setSplineId set id spline
     * @param value id
     * @param id don't show this id in list
     */
    void         setSplinePathId(const quint32 &value, const quint32 &id);
public slots:
    /**
     * @brief ChoosedObject gets id and type of selected object. Save right data and ignore wrong.
     * @param id id of point or detail
     * @param type type of object
     */
    virtual void ChoosedObject(quint32 id, const Valentina::Scenes &type);
    /**
     * @brief DialogAccepted save data and emit signal about closed dialog.
     */
    virtual void DialogAccepted();
    /** TODO ISSUE 79 : create real function
     * @brief DialogApply apply data and emit signal about applied dialog.
     */
    virtual void      DialogApply(){}
private:
    Q_DISABLE_COPY(DialogCutSplinePath)
    /**
     * @brief ui keeps information about user interface
     */
    Ui::DialogCutSplinePath *ui;
    /**
     * @brief pointName name of created point
     */
    QString      pointName;
    /**
     * @brief formula string with formula
     */
    QString      formula;
    /**
     * @brief splinePathId keep id of splinePath
     */
    quint32      splinePathId;
};

inline QString DialogCutSplinePath::getPointName() const
{
    return pointName;
}

inline QString DialogCutSplinePath::getFormula() const
{
    return qApp->FormulaFromUser(formula);
}

inline quint32 DialogCutSplinePath::getSplinePathId() const
{
    return splinePathId;
}

#endif // DIALOGCUTSPLINEPATH_H
