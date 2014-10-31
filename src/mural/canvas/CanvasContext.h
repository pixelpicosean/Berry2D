#pragma once

class CanvasContext
{
    bool needsPresenting;
public:
    bool msaaEnabled;
    int msaaSamples;
    short width;
    short height;

    void create();
    void flushBuffers();
    void prepare();
};
