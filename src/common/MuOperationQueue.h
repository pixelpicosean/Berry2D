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

    void addOperation(MuOperation m);

private:
    // No copy
    MuOperationQueue(const MuOperationQueue&) {}
    void operator=(const MuOperationQueue&) {}

    void run();
};

}
