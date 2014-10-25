#include <cstdio>
#include "mural/AppViewController.h"

int main(int argc, char const *argv[])
{
    theAppController.initWithScriptAtPath(
        "index.js",
        640, 480,
        "Mural Game"
    );

    return 0;
}
