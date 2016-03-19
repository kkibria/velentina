function versionIsAtLeast(actualVersion, expectedVersion)
{
    var actualVersionParts = actualVersion.split('.').map(function(item) {
        return parseInt(item, 10);
    });
    var expectedVersionParts = expectedVersion.split('.').map(function(item) {
        return parseInt(item, 10);
    });

    var msg = "The project requires at least Qt version "+ expectedVersion

    for (var i = 0; i < expectedVersionParts.length; ++i) {
        if (actualVersionParts[i] > expectedVersionParts[i])
            return true;
        if (actualVersionParts[i] < expectedVersionParts[i]) {
            //Console logging API will be supported in Qbs 1.5. So, waiting.
            //console.error(msg);
            //return false;
            throw msg;
        }
    }

    var correct = (i === expectedVersionParts.length || expectedVersionParts[i] === 0);
    if (! correct)
        //Console logging API will be supported in Qbs 1.5. So, waiting.
        //console.error(msg);
        throw msg;

    return correct;
}
