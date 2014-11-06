#include "mural/AppViewController.h"

int main(int argc, char const *argv[])
{
    const char *paths[] = {
        "tests/jasmine.js",
        "tests/jasmine-console.js",
        "tests/jasmine-boot.js",
        "tests/tests.js",
        // "index.js",
        NULL
    };

    theAppController.initWithScripts(
        juce::StringArray(paths),
        640, 480,
        "Mural Game"
    );

    return 0;
}
