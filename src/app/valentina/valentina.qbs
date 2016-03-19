import qbs
import QtcFunctions

CppApplication {
    name: "valentina"
    //minimumQbsVersion: "1.5.0" Waiting The console logging API.
    Depends { name: "Qt.core" }
    property string minimumQtVersion: "5.0.0"
    condition: QtcFunctions.versionIsAtLeast(Qt.core.version, minimumQtVersion)
}
