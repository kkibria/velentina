# ADD TO EACH PATH $$PWD VARIABLE!!!!!!
# This need for corect working file translations.pro

HEADERS += \
    $$PWD/vdatatool.h \
    $$PWD/vabstracttool.h \
    $$PWD/tools.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtooltriangle.h \
    $$PWD/drawTools/toolcurve/vtoolsplinepath.h \
    $$PWD/drawTools/toolcurve/vtoolspline.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolbasepoint.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolshoulderpoint.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersection.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofcontact.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolsinglepoint.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolnormal.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollinepoint.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoollineintersect.h \
    $$PWD/drawTools/vtoolline.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolheight.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolendline.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolbisector.h \
    $$PWD/drawTools/toolcurve/vtoolarc.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolalongline.h \
    $$PWD/drawTools/vdrawtool.h \
    $$PWD/drawTools/drawtools.h \
    $$PWD/nodeDetails/vnodesplinepath.h \
    $$PWD/nodeDetails/vnodespline.h \
    $$PWD/nodeDetails/vnodepoint.h \
    $$PWD/nodeDetails/vnodearc.h \
    $$PWD/nodeDetails/vabstractnode.h \
    $$PWD/nodeDetails/nodedetails.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutspline.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutsplinepath.h \
    $$PWD/vtooluniondetails.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutarc.h \
    $$PWD/drawTools/toolcurve/vabstractspline.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcut.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollineintersectaxis.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolcurveintersectaxis.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectionarcs.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncircles.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointfromcircleandtangent.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointfromarcandtangent.h \
    $$PWD/drawTools/toolcurve/vtoolarcwithlength.h \
    $$PWD/drawTools/toolpoint/vabstractpoint.h \
    $$PWD/drawTools/toolpoint/tooldoublepoint/vtooldoublepoint.h \
    $$PWD/drawTools/toolpoint/tooldoublepoint/vtooltruedarts.h \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncurves.h \
    $$PWD/drawTools/toolcurve/vtoolcubicbezier.h \
    $$PWD/drawTools/toolcurve/vtoolcubicbezierpath.h \
    $$PWD/drawTools/operation/vtoolrotation.h \
    $$PWD/drawTools/operation/flipping/vtoolflippingbyline.h \
    $$PWD/drawTools/operation/vabstractoperation.h \
    $$PWD/drawTools/operation/flipping/vtoolflippingbyaxis.h \
    $$PWD/drawTools/operation/flipping/vabstractflipping.h \
    $$PWD/drawTools/operation/vtoolmove.h \
    $$PWD/drawTools/toolcurve/vtoolellipticalarc.h \
    $$PWD/nodeDetails/vnodeellipticalarc.h \
    $$PWD/vtoolseamallowance.h \
    $$PWD/nodeDetails/vtoolpiecepath.h \
    $$PWD/nodeDetails/vtoolpin.h \
    $$PWD/vinteractivetool.h

SOURCES += \
    $$PWD/vdatatool.cpp \
    $$PWD/vabstracttool.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtooltriangle.cpp \
    $$PWD/drawTools/toolcurve/vtoolsplinepath.cpp \
    $$PWD/drawTools/toolcurve/vtoolspline.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolbasepoint.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolshoulderpoint.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersection.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofcontact.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolsinglepoint.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolnormal.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollinepoint.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoollineintersect.cpp \
    $$PWD/drawTools/vtoolline.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolheight.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolendline.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolbisector.cpp \
    $$PWD/drawTools/toolcurve/vtoolarc.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolalongline.cpp \
    $$PWD/drawTools/vdrawtool.cpp \
    $$PWD/nodeDetails/vnodesplinepath.cpp \
    $$PWD/nodeDetails/vnodespline.cpp \
    $$PWD/nodeDetails/vnodepoint.cpp \
    $$PWD/nodeDetails/vnodearc.cpp \
    $$PWD/nodeDetails/vabstractnode.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutspline.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutsplinepath.cpp \
    $$PWD/vtooluniondetails.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcutarc.cpp \
    $$PWD/drawTools/toolcurve/vabstractspline.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toolcut/vtoolcut.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoollineintersectaxis.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/toollinepoint/vtoolcurveintersectaxis.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectionarcs.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncircles.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointfromcircleandtangent.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointfromarcandtangent.cpp \
    $$PWD/drawTools/toolcurve/vtoolarcwithlength.cpp \
    $$PWD/drawTools/toolpoint/vabstractpoint.cpp \
    $$PWD/drawTools/toolpoint/tooldoublepoint/vtooldoublepoint.cpp \
    $$PWD/drawTools/toolpoint/tooldoublepoint/vtooltruedarts.cpp \
    $$PWD/drawTools/toolpoint/toolsinglepoint/vtoolpointofintersectioncurves.cpp \
    $$PWD/drawTools/toolcurve/vtoolcubicbezier.cpp \
    $$PWD/drawTools/toolcurve/vtoolcubicbezierpath.cpp \
    $$PWD/drawTools/operation/vtoolrotation.cpp \
    $$PWD/drawTools/operation/flipping/vtoolflippingbyline.cpp \
    $$PWD/drawTools/operation/vabstractoperation.cpp \
    $$PWD/drawTools/operation/flipping/vtoolflippingbyaxis.cpp \
    $$PWD/drawTools/operation/flipping/vabstractflipping.cpp \
    $$PWD/drawTools/operation/vtoolmove.cpp \
    $$PWD/drawTools/toolcurve/vtoolellipticalarc.cpp \
    $$PWD/nodeDetails/vnodeellipticalarc.cpp \
    $$PWD/vtoolseamallowance.cpp \
    $$PWD/nodeDetails/vtoolpiecepath.cpp \
    $$PWD/nodeDetails/vtoolpin.cpp \
    $$PWD/vinteractivetool.cpp
