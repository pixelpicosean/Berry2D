#include "MuOperationQueue.h"

namespace mural
{

MuOperationQueue::MuOperationQueue():
    done(false)
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
}

void MuOperationQueue::addOperation(MuOperation m)
{
    this->mq.push(m);
}

void MuOperationQueue::addBlockOperation(MuOperation m)
{
    this->bmq.push(m);
}

void MuOperationQueue::doBlockOperations()
{
    while (this->bmq.size() > 0) {
        MuOperation msg = this->bmq.front();
        bmq.pop();
        msg();
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
