#pragma once

#include <queue>
#include <thread>

namespace mural
{

typedef std::function<void()> MuOperation;

class MuOperationQueue
{
    bool done;
    std::queue<MuOperation> mq;

    std::queue<MuOperation> bmq;
    std::queue<MuOperation> bmq2;
    int currentBmqIdx;

    std::unique_ptr<std::thread> thd;

public:
    MuOperationQueue();
    ~MuOperationQueue();

    /* Operations run on a separate thread */
    void addOperation(MuOperation m);
    /* Operations run on a main thread (for OpenGL...) */
    void addBlockOperation(MuOperation m);

    static MuOperationQueue& defaultQueue()
    {
        static MuOperationQueue instance;
        return instance;
    }

    void doBlockOperations();

private:
    // No copy
    MuOperationQueue(const MuOperationQueue&) {}
    void operator=(const MuOperationQueue&) {}

    void doUnblockOperations();
};

}
