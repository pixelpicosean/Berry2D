#include <cstdio>
#include "berry/AppViewController.h"

int main(int argc, char const *argv[])
{
    berry::AppViewController vc;
    vc.initWithScriptAtPath("index.js");

    return 0;
}
