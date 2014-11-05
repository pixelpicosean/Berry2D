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

    juce::Logger *log = juce::Logger::getCurrentLogger();
    juce::String message("Hello from JUCE default logger");
    log->writeToLog(message);

    theAppController.initWithScripts(
        paths,
        640, 480,
        "Mural Game"
    );

    return 0;
}
