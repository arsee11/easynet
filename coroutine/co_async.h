#ifndef CO_SYNC_H
#define CO_SYNC_H

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{
    
    /*
    template<Socket>
    int async_read(Socket& s, MsgSt& msg){
        auto ret = co_async_read(s, msg);
        return ret.get(); 
    }

    template<Socket>
    int async_write(Socket& s, MsgSt& msg){
        auto ret = co_async_write(s, msg);
        return ret.get(); 
    }

    template<Socket>
    task<int> co_async_read(Socket& s, MsgSt& msg){
        co_await s.async_sread(msg);
        co_return msg.size();
    }

    template<Socket>
    task<int> co_async_write(Socket& s, MsgSt& msg){
        co_await s.asnyc_write(msg);
        co_return msg.size();
    }

    */
}//net
NAMESP_END
#endif /*CO_SYNC_H*/
