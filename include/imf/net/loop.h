#pragma once
#include <assert.h>

#include <thread>
#include <memory>

#include "imf/base/constructormagic.h"
#include "libuv/include/uv.h"

#ifdef _WIN32
#ifdef LIBSSP_EXPORTS
#define LIBSSP_API __declspec(dllexport)
#else
#define LIBSSP_API __declspec(dllimport)
#endif
#pragma warning (disable: 4251)
#else
#define LIBSSP_API
#endif

namespace imf
{

class LIBSSP_API Loop
{
public:
    Loop();
    ~Loop();

    int init(void);
    int loop(void);

    int quit(void);

    void assertInLoopThread(void)
    {
        assert(id_ == std::this_thread::get_id());
    }

    std::thread::id threadId()
    {
        return id_;
    }
    uv_loop_t * uvLoop()
    {
        return &loop_;
    }

private:
    static void onUvAsyncCb(uv_async_t * async);

    uv_loop_t loop_;
    bool run_;
    std::thread::id id_;
    uv_async_t uvAsync_;

    DISALLOW_COPY_AND_ASSIGN(Loop);
};
}
