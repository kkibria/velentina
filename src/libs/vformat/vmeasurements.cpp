/************************************************************************
 **
 **  @file   vmeasurements.cpp
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   14 7, 2015
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

#include "vmeasurements.h"
#include "../ifc/xml/vvstconverter.h"
#include "../ifc/xml/vvitconverter.h"
#include "../ifc/exception/vexceptionemptyparameter.h"
#include "../vpatterndb/calculator.h"

const QString VMeasurements::TagVST              = QStringLiteral("vst");
const QString VMeasurements::TagVIT              = QStringLiteral("vit");
const QString VMeasurements::TagBodyMeasurements = QStringLiteral("body-measurements");
const QString VMeasurements::TagUnit             = QStringLiteral("unit");
const QString VMeasurements::TagNotes            = QStringLiteral("notes");
const QString VMeasurements::TagSize             = QStringLiteral("size");
const QString VMeasurements::TagHeight           = QStringLiteral("height");
const QString VMeasurements::TagPersonal         = QStringLiteral("personal");
const QString VMeasurements::TagFamilyName       = QStringLiteral("family-name");
const QString VMeasurements::TagGivenName        = QStringLiteral("given-name");
const QString VMeasurements::TagBirthDate        = QStringLiteral("birth-date");
const QString VMeasurements::TagSex              = QStringLiteral("sex");
const QString VMeasurements::TagEmail            = QStringLiteral("email");
const QString VMeasurements::TagReadOnly         = QStringLiteral("read-only");
const QString VMeasurements::TagMeasurement      = QStringLiteral("m");

const QString VMeasurements::AttrBase           = QStringLiteral("base");
const QString VMeasurements::AttrValue          = QStringLiteral("value");
const QString VMeasurements::AttrSizeIncrease   = QStringLiteral("size_increase");
const QString VMeasurements::AttrHeightIncrease = QStringLiteral("height_increase");
const QString VMeasurements::AttrDescription    = QStringLiteral("description");
const QString VMeasurements::AttrName           = QStringLiteral("name");
const QString VMeasurements::AttrFullName       = QStringLiteral("full_name");

const QString VMeasurements::SexMale    = QStringLiteral("male");
const QString VMeasurements::SexFemale  = QStringLiteral("female");
const QString VMeasurements::SexUnknown = QStringLiteral("unknown");

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Unknown)
{
    SCASSERT(data != nullptr)
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(Unit unit, VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Individual)
{
    SCASSERT(data != nullptr);

    CreateEmptyIndividualFile(unit);
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::VMeasurements(Unit unit, int baseSize, int baseHeight, VContainer *data)
    :VDomDocument(),
      data(data),
      type(MeasurementsType::Standard)
{
    SCASSERT(data != nullptr);

    CreateEmptyStandardFile(unit, baseSize, baseHeight);
}

//---------------------------------------------------------------------------------------------------------------------
VMeasurements::~VMeasurements()
{
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::setXMLContent(const QString &fileName)
{
    VDomDocument::setXMLContent(fileName);
    type = ReadType();
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::AddEmpty(const QString &name, const QString &formula)
{
    const QDomElement element = MakeEmpty(name, formula);

    const QDomNodeList list = elementsByTagName(TagBodyMeasurements);
    list.at(0).appendChild(element);
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::AddEmptyAfter(const QString &after, const QString &name, const QString &formula)
{
    const QDomElement element = MakeEmpty(name, formula);
    const QDomElement sibling = FindM(after);

    const QDomNodeList list = elementsByTagName(TagBodyMeasurements);

    if (sibling.isNull())
    {
        list.at(0).appendChild(element);
    }
    else
    {
        list.at(0).insertAfter(element, sibling);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::Remove(const QString &name)
{
    const QDomNodeList list = elementsByTagName(TagBodyMeasurements);
    list.at(0).removeChild(FindM(name));
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::MoveUp(const QString &name)
{
    const QDomElement node = FindM(name);
    if (not node.isNull())
    {
        const QDomElement prSibling = node.previousSiblingElement(TagMeasurement);
        if (not prSibling.isNull())
        {
            const QDomNodeList list = elementsByTagName(TagBodyMeasurements);
            list.at(0).insertBefore(node, prSibling);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::MoveDown(const QString &name)
{
    const QDomElement node = FindM(name);
    if (not node.isNull())
    {
        const QDomElement nextSibling = node.nextSiblingElement(TagMeasurement);
        if (not nextSibling.isNull())
        {
            const QDomNodeList list = elementsByTagName(TagBodyMeasurements);
            list.at(0).insertAfter(node, nextSibling);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::ReadMeasurements() const
{
    // For conversion values we must first calculate all data in measurement file's unit.
    // That's why we need two containers: one for converted values, second for real data.

    // Container for values in measurement file's unit
    VContainer *tempData = new VContainer(data->GetTrVars(), data->GetPatternUnit());

    const QDomNodeList list = elementsByTagName(TagMeasurement);
    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement dom = list.at(i).toElement();

        const QString name = GetParametrString(dom, AttrName);

        QString description;
        try
        {
            description = GetParametrString(dom, AttrDescription);
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        QString fullName;
        try
        {
            fullName = GetParametrString(dom, AttrFullName);
        }
        catch (VExceptionEmptyParameter &e)
        {
            Q_UNUSED(e)
        }

        VMeasurement *meash;
        VMeasurement *tempMeash;
        if (type == MeasurementsType::Standard)
        {
            qreal base = GetParametrDouble(dom, AttrBase, "0");
            qreal ksize = GetParametrDouble(dom, AttrSizeIncrease, "0");
            qreal kheight = GetParametrDouble(dom, AttrHeightIncrease, "0");

            tempMeash = new VMeasurement(i, name, BaseSize(), BaseHeight(), base, ksize, kheight);

            base = UnitConvertor(base, MUnit(), *data->GetPatternUnit());
            ksize = UnitConvertor(ksize, MUnit(), *data->GetPatternUnit());
            kheight = UnitConvertor(kheight, MUnit(), *data->GetPatternUnit());

            const qreal baseSize = UnitConvertor(BaseSize(), MUnit(), *data->GetPatternUnit());
            const qreal baseHeight = UnitConvertor(BaseHeight(), MUnit(), *data->GetPatternUnit());

            meash = new VMeasurement(i, name, baseSize, baseHeight, base, ksize, kheight, fullName, description);
        }
        else
        {
            const QString formula = GetParametrString(dom, AttrValue, "0");
            bool ok = false;
            qreal value = EvalFormula(tempData, formula, &ok);

            tempMeash = new VMeasurement(tempData, i, name, value, formula, ok);

            value = UnitConvertor(value, MUnit(), *data->GetPatternUnit());
            meash = new VMeasurement(data, i, name, value, formula, ok, fullName, description);
        }
        tempData->AddVariable(name, tempMeash);
        data->AddVariable(name, meash);
    }

    delete tempData;
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType VMeasurements::Type() const
{
    return type;
}

//---------------------------------------------------------------------------------------------------------------------
Unit VMeasurements::MUnit() const
{
    const QString unit = UniqueTagText(TagUnit, UnitCM);
    return VDomDocument::StrToUnits(unit);
}

//---------------------------------------------------------------------------------------------------------------------
int VMeasurements::BaseSize() const
{
    if (type == MeasurementsType::Standard)
    {
        return static_cast<int>(UniqueTagAttr(TagSize, AttrBase, 50));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
int VMeasurements::BaseHeight() const
{
    if (type == MeasurementsType::Standard)
    {
        return static_cast<int>(UniqueTagAttr(TagHeight, AttrBase, 176));
    }
    else
    {
        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::Notes() const
{
    return UniqueTagText(TagNotes, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetNotes(const QString &text)
{
    if (not ReadOnly())
    {
        setTagText(TagNotes, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::FamilyName() const
{
    return UniqueTagText(TagFamilyName, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetFamilyName(const QString &text)
{
    if (not ReadOnly())
    {
        setTagText(TagFamilyName, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::GivenName() const
{
    return UniqueTagText(TagGivenName, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetGivenName(const QString &text)
{
    if (not ReadOnly())
    {
        setTagText(TagGivenName, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QDate VMeasurements::BirthDate() const
{
    const QString date = UniqueTagText(TagBirthDate, "1990-02-15");
    return QDate::fromString(date, "yyyy-MM-dd");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetBirthDate(const QDate &date)
{
    if (not ReadOnly())
    {
        setTagText(TagBirthDate, date.toString("yyyy-MM-dd"));
    }
}

//---------------------------------------------------------------------------------------------------------------------
SexType VMeasurements::Sex() const
{
    return StrToGender(UniqueTagText(TagSex, ""));
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetSex(const SexType &sex)
{
    if (not ReadOnly())
    {
        setTagText(TagSex, GenderToStr(sex));
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::Email() const
{
    return UniqueTagText(TagEmail, "");
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetEmail(const QString &text)
{
    if (not ReadOnly())
    {
        setTagText(TagEmail, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
bool VMeasurements::ReadOnly() const
{
    if (UniqueTagText(TagReadOnly, "false") == "true")
    {
        return true;
    }
    else
    {
        return false;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetReadOnly(bool ro)
{
    if (ro)
    {
        setTagText(TagReadOnly, "true");
    }
    else
    {
        setTagText(TagReadOnly, "false");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMName(const QString &name, const QString &text)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrName, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMValue(const QString &name, const QString &text)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrValue, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMBaseValue(const QString &name, double value)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrBase, value);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMSizeIncrease(const QString &name, double value)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrSizeIncrease, value);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMHeightIncrease(const QString &name, double value)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrHeightIncrease, value);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMDescription(const QString &name, const QString &text)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrDescription, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetMFullName(const QString &name, const QString &text)
{
    QDomElement node = FindM(name);
    if (not node.isNull())
    {
        SetAttribute(node, AttrFullName, text);
    }
}

//---------------------------------------------------------------------------------------------------------------------
QString VMeasurements::GenderToStr(const SexType &sex)
{
    switch (sex)
    {
        case SexType::Male:
            return SexMale;
        case SexType::Female:
            return SexFemale;
        case SexType::Unknown:
            return SexUnknown;
        default:
            return SexUnknown;
    }
}

//---------------------------------------------------------------------------------------------------------------------
SexType VMeasurements::StrToGender(const QString &sex)
{
    QStringList genders = QStringList() << SexMale << SexFemale << SexUnknown;
    switch (genders.indexOf(sex))
    {
        case 0: // SexMale
            return SexType::Male;
        case 1: // SexFemale
            return SexType::Female;
        case 2: // SexUnknown
            return SexType::Unknown;
        default:
            return SexType::Unknown;
    }
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurements::ListAll() const
{
    QStringList listNames;
    const QDomNodeList list = elementsByTagName(TagMeasurement);

    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement domElement = list.at(i).toElement();
        if (domElement.isNull() == false)
        {
            listNames.append(domElement.attribute(AttrName));
        }
    }

    return listNames;
}

//---------------------------------------------------------------------------------------------------------------------
QStringList VMeasurements::ListKnown() const
{
    QStringList listNames;
    const QStringList list = ListAll();
    for (int i=0; i < list.size(); ++i)
    {
        if (list.at(i).indexOf(CustomMSign) != 0)
        {
            listNames.append(list.at(i));
        }
    }

    return listNames;
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetDataSize()
{
    data->SetSize(UnitConvertor(BaseSize(), MUnit(), *data->GetPatternUnit()));
    data->SetSizeName(size_M);
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::SetDataHeight()
{
    data->SetHeight(UnitConvertor(BaseHeight(), MUnit(), *data->GetPatternUnit()));
    data->SetHeightName(height_M);
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::CreateEmptyStandardFile(Unit unit, int baseSize, int baseHeight)
{
    this->clear();
    QDomElement mElement = this->createElement(TagVST);

    mElement.appendChild(createComment("Measurements created with Valentina (http://www.valentina-project.org/)."));

    QDomElement version = createElement(TagVersion);
    const QDomText newNodeText = createTextNode(VVSTConverter::MeasurementMaxVerStr);
    version.appendChild(newNodeText);
    mElement.appendChild(version);

    QDomElement ro = createElement(TagReadOnly);
    const QDomText roNodeText = createTextNode("false");
    ro.appendChild(roNodeText);
    mElement.appendChild(ro);

    mElement.appendChild(createElement(TagNotes));

    QDomElement mUnit = createElement(TagUnit);
    const QDomText unitText = createTextNode(UnitsToStr(unit));
    mUnit.appendChild(unitText);
    mElement.appendChild(mUnit);

    QDomElement size = createElement(TagSize);
    SetAttribute(size, AttrBase, QString().setNum(baseSize));
    mElement.appendChild(size);

    QDomElement height = createElement(TagHeight);
    SetAttribute(height, AttrBase, QString().setNum(baseHeight));
    mElement.appendChild(height);

    mElement.appendChild(createElement(TagBodyMeasurements));

    this->appendChild(mElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
void VMeasurements::CreateEmptyIndividualFile(Unit unit)
{
    this->clear();
    QDomElement mElement = this->createElement(TagVIT);

    mElement.appendChild(createComment("Measurements created with Valentina (http://www.valentina-project.org/)."));

    QDomElement version = createElement(TagVersion);
    const QDomText newNodeText = createTextNode(VVITConverter::MeasurementMaxVerStr);
    version.appendChild(newNodeText);
    mElement.appendChild(version);

    QDomElement ro = createElement(TagReadOnly);
    const QDomText roNodeText = createTextNode("false");
    ro.appendChild(roNodeText);
    mElement.appendChild(ro);

    mElement.appendChild(createElement(TagNotes));

    QDomElement mUnit = createElement(TagUnit);
    mUnit.appendChild(createTextNode(UnitsToStr(unit)));
    mElement.appendChild(mUnit);

    QDomElement personal = createElement(TagPersonal);
    personal.appendChild(createElement(TagFamilyName));
    personal.appendChild(createElement(TagGivenName));

    QDomElement date = createElement(TagBirthDate);
    const QDomText newDate = createTextNode(QDate(1990, 02, 15).toString("yyyy-MM-dd"));
    date.appendChild(newDate);
    personal.appendChild(date);

    QDomElement sex = createElement(TagSex);
    const QDomText newSex = createTextNode(GenderToStr(SexType::Male));
    sex.appendChild(newSex);
    personal.appendChild(sex);

    personal.appendChild(createElement(TagEmail));
    mElement.appendChild(personal);

    mElement.appendChild(createElement(TagBodyMeasurements));

    this->appendChild(mElement);
    insertBefore(createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\""), this->firstChild());
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurements::UniqueTagAttr(const QString &tag, const QString &attr, qreal defValue) const
{
    const qreal defVal = UnitConvertor(defValue, Unit::Cm, MUnit());

    const QDomNodeList nodeList = this->elementsByTagName(tag);
    if (nodeList.isEmpty())
    {
        return defVal;
    }
    else
    {
        const QDomNode domNode = nodeList.at(0);
        if (domNode.isNull() == false && domNode.isElement())
        {
            const QDomElement domElement = domNode.toElement();
            if (domElement.isNull() == false)
            {
                return GetParametrDouble(domElement, attr, QString("%1").arg(defVal));
            }
        }
    }
    return defVal;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VMeasurements::MakeEmpty(const QString &name, const QString &formula)
{
    QDomElement element = createElement(TagMeasurement);

    SetAttribute(element, AttrName, name);

    if (type == MeasurementsType::Standard)
    {
        SetAttribute(element, AttrBase, QString("0"));
        SetAttribute(element, AttrSizeIncrease, QString("0"));
        SetAttribute(element, AttrHeightIncrease, QString("0"));
    }
    else
    {
        if (formula.isEmpty())
        {
            SetAttribute(element, AttrValue, QString("0"));
        }
        else
        {
            SetAttribute(element, AttrValue, formula);
        }
        SetAttribute(element, AttrDescription, QString(""));
        SetAttribute(element, AttrFullName, QString(""));
    }

    return element;
}

//---------------------------------------------------------------------------------------------------------------------
QDomElement VMeasurements::FindM(const QString &name) const
{
    QDomNodeList list = elementsByTagName(TagMeasurement);

    for (int i=0; i < list.size(); ++i)
    {
        const QDomElement domElement = list.at(i).toElement();
        if (domElement.isNull() == false)
        {
            const QString parameter = domElement.attribute(AttrName);
            if (parameter == name)
            {
                return domElement;
            }
        }
    }

    return QDomElement();
}

//---------------------------------------------------------------------------------------------------------------------
MeasurementsType VMeasurements::ReadType() const
{
    QDomElement root = documentElement();
    if (root.tagName() == TagVST)
    {
        return MeasurementsType::Standard;
    }
    else if (root.tagName() == TagVIT)
    {
        return MeasurementsType::Individual;
    }
    else
    {
        return MeasurementsType::Unknown;
    }
}

//---------------------------------------------------------------------------------------------------------------------
qreal VMeasurements::EvalFormula(VContainer *data, const QString &formula, bool *ok) const
{
    if (formula.isEmpty())
    {
        *ok = true;
        return 0;
    }
    else
    {
        try
        {
            // Replace line return character with spaces for calc if exist
            QString f = formula;
            f.replace("\n", " ");
            Calculator *cal = new Calculator(data, type);
            const qreal result = cal->EvalFormula(f);
            delete cal;

            *ok = true;
            return result;
        }
        catch (qmu::QmuParserError &e)
        {
            *ok = false;
            return 0;
        }
    }
}
