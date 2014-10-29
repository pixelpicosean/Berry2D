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
    std::unique_ptr<std::thread> thd;

public:
    MuOperationQueue();
    ~MuOperationQueue();

    /* Operations run on a separate thread */
    void addOperation(MuOperation m);

    static MuOperationQueue& defaultQueue()
    {
        static MuOperationQueue instance;
        return instance;
    }

private:
    // No copy
    MuOperationQueue(const MuOperationQueue&) {}
    void operator=(const MuOperationQueue&) {}

    void run();
};

}
