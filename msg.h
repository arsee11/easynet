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

    //construct with @capicity of bytes
	explicit Msg(size_t capicity)
		:_capicity(capicity)
	{
		_buf.reset( new uint8_t[capicity] );
		_rd_ptr = _buf.get();
		_wr_ptr = _buf.get();
	}

    //construct with  @size of @data, make a copy of @data.
    explicit Msg(const void* data, size_t size)
       :_capicity(size)
    {
    	_buf.reset( new uint8_t[size] );
    	memcpy(_buf.get(), data, size);
		_rd_ptr = _buf.get();
		_wr_ptr = _buf.get()+size;
    }

    uint8_t* rd_ptr()const{ return _rd_ptr; }
    uint8_t* wr_ptr(){ return _wr_ptr; }
	size_t size()const{ 
		return _wr_ptr - _rd_ptr;
	}

	void size(int val){ 
		_wr_ptr += val;
	}

	void consume(size_t nbytes){
		if(nbytes <= size()){
			_rd_ptr += nbytes;
		}
		else{
			_rd_ptr += size();
		}
	}

	size_t capicity()const{ return _capicity; }
private:
	std::shared_ptr<uint8_t> _buf;	
	uint8_t* _rd_ptr=nullptr;
	uint8_t* _wr_ptr=nullptr;
    size_t _capicity=0;
};


///single thread msg;
using MsgSt = Msg<false>;

}//net
NAMESP_END

#endif/*MSG_H*/
