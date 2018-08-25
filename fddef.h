//fddef.h

#ifndef FDDEF_H
#define FDDEF_H

#include "namespdef.h"

NAMESP_BEGIN
namespace net
{

#ifdef _MSC_VER
typedef SOCKET fd_t;
#endif

#ifdef __GNUC__
typedef int fd_t;
#endif

}//net
NAMESP_END

#endif/*FD_DEF_H*/
