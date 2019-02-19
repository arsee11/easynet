///msg.h
///

#ifndef MSG_H
#define MSG_H

#include <memory>
#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

template<bool isMultiThreads>
class Msg;

template<>
class Msg<false>
{
public:
	Msg(){};

        //construct with @maxsize of bytes
	explicit Msg(size_t maxsize)
		:_maxsize(maxsize)
	{
		_buf.reset( new uint8_t[maxsize] );
	}

        //construct with  @size of @data, make a copy of @data.
        explicit Msg(const void* data, size_t size)
                :_maxsize(size)
                ,_size(size)
        {
                _buf.reset( new uint8_t[size] );
                memcpy(_buf.get(), data, size);
        }

        uint8_t* begin()const{ return _buf.get(); }
        uint8_t* end()const{ return _buf.get() + _size; }
	size_t size()const{ return _size; }
	void size(size_t val){ _size=val; }

	size_t maxsize()const{ return _maxsize; }
private:
	std::shared_ptr<uint8_t> _buf;	

        size_t _maxsize=0;
        size_t _size=0;
};


///single thread msg;
using MsgSt = Msg<false>;

}//net
NAMESP_END

#endif/*MSG_H*/
