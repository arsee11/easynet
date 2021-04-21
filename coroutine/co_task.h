#ifndef CO_TASK_H
#define CO_TASK_H

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

struct CoTask{
  struct promise_type {
    CoTask get_return_object() { return {}; }
    std::suspend_never initial_suspend() { return {}; }
    std::suspend_never final_suspend() noexcept { return {}; }
    void return_void() {}
    void unhandled_exception() {}
  };
};

}//net
NAMESP_END
#endif /*CO_TASK_H*/
