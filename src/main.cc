#include "mural/AppViewController.h"

int main(int argc, char const *argv[])
{
    theAppController.initWithScripts({
            "tests/jasmine.js",
            "tests/jasmine-console.js",
            "tests/jasmine-boot.js",
            "tests/tests.js",
            // "index.js",
        },
        640, 480,
        "Mural Game"
    );

    return 0;
}
