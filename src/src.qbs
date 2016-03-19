import qbs

Project {
    name: "Sources"
	references: [
        "app/app.qbs",
        "libs/libs.qbs",
        "test/test.qbs"
    ]
}
