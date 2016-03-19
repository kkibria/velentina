import qbs

// qbs build noTests:false
Project {
	name: "Tests"
	condition: noTests === false
    references: [
        "ParserTest/ParserTest.qbs",
        "ValentinaTest/ValentinaTest.qbs"
    ]
}
