#ifndef CO_SYNC_H
#define CO_SYNC_H

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{
    
    template<Socket>
    task<int> co_async_read(Socket& s, MsgSt& msg);

    template<Socket>
    task<int> co_async_write(Socket& s, MsgSt& msg);

}//net
NAMESP_END
#endif /*CO_SYNC_H*/
