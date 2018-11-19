#pragma once

#include <functional>
#include <thread>
#include <memory>

#include "loop.h"

#include "imf/base/constructormagic.h"

namespace imf
{

class ThreadLoop
{
public:
    typedef std::function<void(Loop*)> PreLoopCallback;
    explicit ThreadLoop(const PreLoopCallback& pre = PreLoopCallback())
        : preLoopCb_(pre),
          started_(false)
    {

    }

    ~ThreadLoop()
    {
        // Buggy
        if (started_) {
            loop_->quit();
            thread_.join();
        }
    }

    void start(void)
    {
        if (started_ == false) {
            started_ = true;
            thread_ = std::thread(&ThreadLoop::run, this);
        }
    }

    void stop(void)
    {
        started_ = false;
        loop_->quit();
        thread_.join();
    }
private:
    void run(void)
    {
        loop_.reset(new Loop());
        loop_->init();

        if (preLoopCb_) {
            preLoopCb_(loop_.get());
        }
        loop_->loop();
    }

    PreLoopCallback preLoopCb_;
    bool started_;
    std::thread thread_;
    std::unique_ptr<Loop> loop_;

    DISALLOW_COPY_AND_ASSIGN(ThreadLoop);
};
}