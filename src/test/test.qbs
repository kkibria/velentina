import qbs

//On the command line, set it via project.<propertyName>:<value>
//qbs build project.noTests:true ...
Project {
    name: "Tests"

    condition: noTests === false
    references: [
        "ParserTest/ParserTest.qbs",
        "ValentinaTest/ValentinaTest.qbs"
    ]
}
