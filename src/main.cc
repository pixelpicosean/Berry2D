#include "mural/AppViewController.h"

int main(int argc, char const *argv[])
{
    StringList paths = {
        "tests/jasmine.js",
        "tests/jasmine-console.js",
        "tests/jasmine-boot.js",
        "tests/tests.js"
        // "index.js"
    };
    theAppController.initWithScripts(
        paths,
        640, 480,
        "Mural Game"
    );

    return 0;
}
