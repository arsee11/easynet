//fddef.h

#ifndef FDDEF_H
#define FDDEF_H

#ifndef NAMESPDEF_H
#include "../namespdef.h"
#endif

NAMESP_BEGIN
namespace net
{

#ifdef _MVC_VER
typedef SOCKET fd_t;
#endif

#ifdef __GNUC__
typedef int fd_t;
#endif

}//net
NAMESP_END

#endif/*FD_DEF_H*/
