import qbs
import QtcFunctions

CppApplication {
    name: "tape"
    //minimumQbsVersion: "1.5.0" Waiting The console logging API.
    Depends { name: "Qt"; submodules: ["core", "gui", "widgets", "network", "xml", "xmlpatterns", "printsupport", "svg"] }
    property string minimumQtVersion: "5.0.0"
    condition: QtcFunctions.versionIsAtLeast(Qt.core.version, minimumQtVersion)
    type: ["application"]

    files: [
        "dialogs/configpages/tapeconfigurationpage.cpp",
        "dialogs/configpages/tapeconfigurationpage.h",
        "dialogs/configpages/tapepathpage.cpp",
        "dialogs/configpages/tapepathpage.h",
        "dialogs/dialogabouttape.cpp",
        "dialogs/dialogabouttape.h",
        "dialogs/dialogabouttape.ui",
        "dialogs/dialogmdatabase.cpp",
        "dialogs/dialogmdatabase.h",
        "dialogs/dialogmdatabase.ui",
        "dialogs/dialognewmeasurements.cpp",
        "dialogs/dialognewmeasurements.h",
        "dialogs/dialognewmeasurements.ui",
        "dialogs/tapeconfigdialog.cpp",
        "dialogs/tapeconfigdialog.h",
        "main.cpp",
        "mapplication.cpp",
        "mapplication.h",
        "share/resources/tapeicon.qrc",
        "stable.h",
        "tmainwindow.cpp",
        "tmainwindow.h",
        "tmainwindow.ui",
        "version.h",
        "vlitepattern.cpp",
        "vlitepattern.h",
        "vtablesearch.cpp",
        "vtablesearch.h",
    ]

    Group {
        name: "PCH Windows"
        condition: qbs.targetOS.contains("windows")
        files: "stable.cpp"
    }

    Group {
        name: "Windows RC"
        condition: qbs.targetOS.contains("windows")
        files: "tape.rc"
    }

    Group {
        name: "Measurement diagram"
        files: 'share/resources/diagrams.qrc'
        fileTags: ['diagram']
    }

    Rule {
        inputs: ["diagram"]
        Artifact {
            filePath: "diagrams.rcc"
            fileTags: "processed_diagram"
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "Making external binary resource";
            cmd.highlight = 'filegen';
            cmd.sourceCode = function() {
                var rccPath = product.moduleProperty("Qt.core", "binPath") + '/rcc'
                var process = new Process();
                var exitCode = process.exec(rccPath, ["-binary", "-no-compress", input.filePath, "-o", output.filePath]);
                if (exitCode !== 0)
                    throw Error("rcc failed with exit code " + exitCode);
            }
            return cmd;
        }
    }
}
