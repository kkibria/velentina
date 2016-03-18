import qbs

Project {
name: "Libs"
    references: [
    	"qmuparser/qmuparser.qbs",
    	"vpropertyexplorer/vpropertyexplorer.qbs",
    	"ifc/ifc.qbs",
    	"vobj/vobj.qbs",
    	"vdxf/vdxf.qbs",
    	"vlayout/vlayout.qbs",
    	"vgeometry/vgeometry.qbs",
    	"vpatterndb/vpatterndb.qbs",
    	"vmisc/vmisc.qbs",
    	"vwidgets/vwidgets.qbs",
    	"vtools/vtools.qbs",
    	"vformat/vformat.qbs"
    ]
}
