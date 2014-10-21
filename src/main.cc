#include <cstdio>
#include "berry/AppViewController.h"

int main(int argc, char const *argv[])
{
    theAppController.initWithScriptAtPath(
        "index.js",
        640, 480,
        "Berry2D"
    );

    return 0;
}
