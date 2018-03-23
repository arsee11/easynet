///connector.h
//
//

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include "addr.h"
#include "netpeer.h"

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

class Connector
{
public:
	Connector(uint16_t local_port){
		_local_addr.ip = "";
		_local_addr.port = local_port;
	}

	Connector(const AddrPair& local_addr)
		:_local_addr(local_addr)
	{}

	virtual NetPeer* connect(const AddrPair& remote_addr)=0;

protected:
	AddrPair _local_addr;
};



/////////////////////////////////////////////////////////////
class TCPConnector : public Connector
{
public:
	TCPConnector(uint16_t local_port)
		:Connector(local_port)
	{
	}

	TCPConnector(const AddrPair& local_addr)
		:Connector(local_addr)
	{}

	NetPeer* connect(const AddrPair& remot_addr)override;
};

}//net
NAMESP_END

#endif /*CONNECTOR_H*/
