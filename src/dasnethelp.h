#ifndef DASNETHELP_H
#define DASNETHELP_H

/*##
##
##contain help functions for interacting with the bsd sockets api
##
##---*/

#include "whichos.h"

//http://tangentsoft.net/wskfaq/articles/bsd-compatibility.html
#ifdef OS_WINDOWS
/*
    getaddrinfo() is not very compatible with versions of windows older than vista.
    however, in XP's SP2, Ws2_32.dll contain the definition of getaddrinfo(),
    and it can be referenced using:

      #define WindowsXP 0x501
      #define WINVER WindowsXP
      #include <winsock2.h>
      #include <ws2tcpip.h>
    
    the downside however, is, the resulting binary will only work on a computer running XP.

    although as i read anew the 1st Source- getaddrinfo() is indeed available on XP-
    regardless of service pack; it is the GetAddrInfoW() that is not available until SP2-
    which is a unicode version of getaddrinfo(). however older versions of windows don't have
    getaddrinfo() and will require something extra which is explained in the 1st Source.

    but i cannot remember why i have to define "WINVER" for getaddrinfo()'s definition to
    be recognized on XP. it just doesn't make sense anymore... so... i'll just do it becuase
    it works, but, if the version is not XP, then there is no need to do it. in which case
    the computer would not be running XP and using only XP's feature set would be not really
    required. what really sucks is that if the version is XP, then, it must be specified that
    it is. so... if one wants to compile for XP, then, one must compile solely for XP, and,
    a user must explicitly define a symbol representing XP:

      #define WINVER 0x501
      #define _WIN32_WINNT 0x501

    either works. //make -DOS_WINDOWS_XP -f src/mingw32-makefile

    https://msdn.microsoft.com/en-us/library/windows/desktop/ms738520(v=vs.85).aspx
    http://mingw.5.n7.nabble.com/Undefined-reference-to-getaddrinfo-td5694.html
    https://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx#setting_winver_or__win32_winnt
    https://msdn.microsoft.com/en-us/library/windows/desktop/aa383745(v=vs.85).aspx#macros_for_conditional_declarations
  
    XP does not make available inet_ntop() or inet_pton()

      WSAAddressToString() //this one adds port to the string if the port number is included in the address
      WSAStringToAddress() //

    VISTA and onwards makes available InetNtop() and InetPton()

    though, i found a wrapper solution; inet_ntop() and inet_pton() will work on windows XP and later
    https://www.mail-archive.com/users@ipv6.org/msg02107.html
*/
  //#define _WIN32_WINNT 0x501
  #include <winsock2.h>
  #include <ws2tcpip.h>
  //#ifdef OS_WINDOWS_XP
  //  #define inet_ntop(a,b,c,d) WSAAddressToString(a,b,c,d)
  //  #define inet_pton(a,b,c,d) WSAStringToAddress(a,b,c,d)
  //#else
  //  #error "please implement a solution for: inet_ntop() and inet_pton()"
  //  #define inet_ntop(a,b,c,d) InetNtop(a,b,c,d)
  //  #define inet_pton(a,b,c,d) InetPton(a,b,c,d)
  //#endif
  const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt);
  int inet_pton(int af, const char *src, void *dst);
  /////// MAKEWORD(1,1) //winsock 1.1
  /////// MAKEWORD(2,0) //winsock 2.0
  /////// WSAData wsaData;
  #define dasnethelps_init() WSAStartup(MAKEWORD(2,0),&((WSADATA){}))
  #define dasnethelps_quit() WSACleanup()
#else
  #include <sys/types.h>   //
  #include <sys/socket.h>  //
  #include <netdb.h>       //getaddrinfo
  #include <ifaddrs.h>     //
  #include <netinet/in.h>  //
  #include <arpa/inet.h>   //
  #define dasnethelps_init() 0
  #define dasnethelps_quit() 0
#endif

#include <stdlib.h>
#include <stdio.h>

/* return port, convert addr->sin[6]_addr to presentable ip address stored in ipaddr_buf_presentable */
int sockaddr_inX_to_presentable(int ai_family, struct sockaddr *addr, char *ipaddr_buf_presentable);
/* perror() equivalent taking to consideration winsock's nil use of errno */
//void dasnethelps_perror(char *prefix);
#ifdef OS_WINDOWS
  #define dasnethelps_perror(prefix) fprintf(stderr,"%s: %s",prefix,WSAGetLastErrorMessage())
#else
  #define dasnethelps_perror(prefix) perror(prefix)
#endif

#ifndef OS_WINDOWS
/* return ip address (presentable: char *) associated with the network interface specified in iface (NULL on error) */
char *getifaceaddr(char *iface);
#endif

#ifdef OS_WINDOWS
/* bsd sockets, winsock does not use errno, WSAGetLastError() is its substitute but perror() does therefore not work for retrieving an error message, however.  WSAGetLastErrorMessage() will return a string formatted error message corresponding to the numerical error value returned by WSAGetLastError().  http://tangentsoft.net/wskfaq/examples/basics/ws-util.cpp */
char *WSAGetLastErrorMessage();
#endif

#endif
