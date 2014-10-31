#include "MuOperationQueue.h"

namespace mural
{

MuOperationQueue::MuOperationQueue():
    done(false),
    currentBmqIdx(1)
{
    this->thd = std::unique_ptr<std::thread>(new std::thread([=] {
        this->doUnblockOperations();
    }));
}

MuOperationQueue::~MuOperationQueue()
{
    this->addOperation([&]{
        done = true;
    });
    this->thd->join();

    // TODO: complete all block operations
}

void MuOperationQueue::addOperation(MuOperation m)
{
    this->mq.push(m);
}

void MuOperationQueue::addBlockOperation(MuOperation m)
{
    if (this->currentBmqIdx == 1) {
        this->bmq.push(m);
    }
    else {
        this->bmq2.push(m);
    }
}

void MuOperationQueue::doBlockOperations()
{
    if (this->currentBmqIdx == 1) {
        // Swap queue
        this->currentBmqIdx = 2;

        while (this->bmq.size() > 0) {
            MuOperation msg = this->bmq.front();
            bmq.pop();
            msg();
        }
    }
    else {
        // Swap queue
        this->currentBmqIdx = 1;

        while (this->bmq2.size() > 0) {
            MuOperation msg = this->bmq2.front();
            bmq2.pop();
            msg();
        }
    }
}

void MuOperationQueue::doUnblockOperations()
{
    while (!done) {
        if (this->mq.size() > 0) {
            MuOperation msg = this->mq.front();
            mq.pop();
            msg();
        }
    } // note: last message sets done to true
}

}
