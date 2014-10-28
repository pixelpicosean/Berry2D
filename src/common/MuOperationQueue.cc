#include "MuOperationQueue.h"

namespace mural
{

MuOperationQueue::MuOperationQueue():
    done(false)
{
    this->thd = std::unique_ptr<std::thread>(new std::thread([=] {
        this->run();
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

void MuOperationQueue::run()
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
