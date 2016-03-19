import qbs 1.0

Project {
    name: "Valentina"
    qbsSearchPaths: "share/qbs-stuff"
    property bool noTests: false

    references: [
        "src/src.qbs"
    ]

    // Trigger the autotest execution by "building" the product: qbs build -p autotest-runner
    AutotestRunner {}
}
