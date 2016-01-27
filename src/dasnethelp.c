#include "dasnethelp.h"

//https://msdn.microsoft.com/en-us/library/windows/desktop/ms738532(v=vs.85).aspx
#ifdef OS_WINDOWS

//https://www.mail-archive.com/users@ipv6.org/msg02107.html
const char *inet_ntop(int af, const void *src, char *dst, socklen_t cnt)
{
  if (af == AF_INET)
  {
    struct sockaddr_in in;
    memset(&in, 0, sizeof(in));
    in.sin_family = AF_INET;
    memcpy(&in.sin_addr, src, sizeof(struct in_addr));
    getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in), dst, cnt, NULL, 0, NI_NUMERICHOST);
    return dst;
  }
  else if (af == AF_INET6)
  {
    struct sockaddr_in6 in;
    memset(&in, 0, sizeof(in));
    in.sin6_family = AF_INET6;
    memcpy(&in.sin6_addr, src, sizeof(struct in_addr6));
    getnameinfo((struct sockaddr *)&in, sizeof(struct sockaddr_in6), dst, cnt, NULL, 0, NI_NUMERICHOST);
    return dst;
  }
  return NULL;
}

//https://www.mail-archive.com/users@ipv6.org/msg02107.html
int inet_pton(int af, const char *src, void *dst)
{
  struct addrinfo hints, *res, *ressave;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = af;
  
  if (getaddrinfo(src, NULL, &hints, &res) != 0)
  {
    //dolog(LOG_ERR, "Couldn't resolve host %s\n", src);
    return -1;
  }
  
  ressave = res;
  
  while (res)
  {
    memcpy(dst, res->ai_addr, res->ai_addrlen);
    res = res->ai_next;
  }
  
  freeaddrinfo(ressave);
  return 0;
}

#endif


/* return port, convert addr->sin[6]_addr to presentable ip address stored in ipaddr_buf_presentable */
int sockaddr_inX_to_presentable(int ai_family, struct sockaddr *addr, char *ipaddr_buf_presentable)
{
  int port;
  if(ai_family == AF_INET)
  {
    struct sockaddr_in *inaddr = (struct sockaddr_in*)addr;
    //#if WINVER == WindowsXP
    //#warn "IPv6 addresses will not work because there is no inet_ntop() function."
    //#ifdef OS_WINDOWS
    //ipaddr_buf_presentable = inet_ntoa(inaddr->sin_addr);
    //#else
    inet_ntop(AF_INET,&inaddr->sin_addr.s_addr,ipaddr_buf_presentable,INET_ADDRSTRLEN);
    //#endif
    port = inaddr->sin_port;
  }
  //#ifndef OS_WINDOWS
  else
  if(ai_family == AF_INET6)
  {
    struct sockaddr_in6 *in6addr = (struct sockaddr_in6*)addr;
    inet_ntop(AF_INET6,&in6addr->sin6_addr,ipaddr_buf_presentable,INET6_ADDRSTRLEN);
    port = in6addr->sin6_port;
  }
  //#endif
  port = ntohs(port);
  //printf("%d\n",port);
  return port;
}

/* this might be Linux specific */
#ifndef OS_WINDOWS
/* return presentable ip address associated with the network interface specified in iface (NULL on error) http://stackoverflow.com/questions/212528/get-the-ip-address-of-the-machine */
char *getifaceaddr(char *iface)
{
  struct ifaddrs * ifAddrStruct=NULL;
  struct ifaddrs * ifa=NULL;
  void * tmpAddrPtr=NULL;
  
  getifaddrs(&ifAddrStruct);
  
  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
      if (!ifa->ifa_addr) {
          continue;
      }
      if(strcmp(iface,ifa->ifa_name))
      {
        continue;
      }
      if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
          // is a valid IP4 Address
          tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
          //char addressBuffer[INET_ADDRSTRLEN];
          char *addressBuffer = malloc(INET_ADDRSTRLEN);
          inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
          //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
          return addressBuffer;
      } else if (ifa->ifa_addr->sa_family == AF_INET6) { // check it is IP6
          // is a valid IP6 Address
          tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
          //char addressBuffer[INET6_ADDRSTRLEN];
          char *addressBuffer = malloc(INET6_ADDRSTRLEN);
          inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer, INET6_ADDRSTRLEN);
          //printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
          return addressBuffer;
      } 
  }
  if (ifAddrStruct!=NULL) freeifaddrs(ifAddrStruct);
  return NULL;
}
#endif

#ifdef OS_WINDOWS
char *WSAGetLastErrorMessage()
{
    char *msg = "No error";
    int error = WSAGetLastError();

    #define S(SYMBOL) if(error == SYMBOL) 
    //http://tangentsoft.net/wskfaq/examples/basics/ws-util.cpp
    S(0)                  msg = "No error";
    S(WSAEINTR)           msg = "Interrupted system call";
    S(WSAEBADF)           msg = "Bad file number";
    S(WSAEACCES)          msg = "Permission denied";
    S(WSAEFAULT)          msg = "Bad address";
    S(WSAEINVAL)          msg = "Invalid argument";
    S(WSAEMFILE)          msg = "Too many open sockets";
    S(WSAEWOULDBLOCK)     msg = "Operation would block";
    S(WSAEINPROGRESS)     msg = "Operation now in progress";
    S(WSAEALREADY)        msg = "Operation already in progress";
    S(WSAENOTSOCK)        msg = "Socket operation on non-socket";
    S(WSAEDESTADDRREQ)    msg = "Destination address required";
    S(WSAEMSGSIZE)        msg = "Message too long";
    S(WSAEPROTOTYPE)      msg = "Protocol wrong type for socket";
    S(WSAENOPROTOOPT)     msg = "Bad protocol option";
    S(WSAEPROTONOSUPPORT) msg = "Protocol not supported";
    S(WSAESOCKTNOSUPPORT) msg = "Socket type not supported";
    S(WSAEOPNOTSUPP)      msg = "Operation not supported on socket";
    S(WSAEPFNOSUPPORT)    msg = "Protocol family not supported";
    S(WSAEAFNOSUPPORT)    msg = "Address family not supported";
    S(WSAEADDRINUSE)      msg = "Address already in use";
    S(WSAEADDRNOTAVAIL)   msg = "Can't assign requested address";
    S(WSAENETDOWN)        msg = "Network is down";
    S(WSAENETUNREACH)     msg = "Network is unreachable";
    S(WSAENETRESET)       msg = "Net connection reset";
    S(WSAECONNABORTED)    msg = "Software caused connection abort";
    S(WSAECONNRESET)      msg = "Connection reset by peer";
    S(WSAENOBUFS)         msg = "No buffer space available";
    S(WSAEISCONN)         msg = "Socket is already connected";
    S(WSAENOTCONN)        msg = "Socket is not connected";
    S(WSAESHUTDOWN)       msg = "Can't send after socket shutdown";
    S(WSAETOOMANYREFS)    msg = "Too many references, can't splice";
    S(WSAETIMEDOUT)       msg = "Connection timed out";
    S(WSAECONNREFUSED)    msg = "Connection refused";
    S(WSAELOOP)           msg = "Too many levels of symbolic links";
    S(WSAENAMETOOLONG)    msg = "File name too long";
    S(WSAEHOSTDOWN)       msg = "Host is down";
    S(WSAEHOSTUNREACH)    msg = "No route to host";
    S(WSAENOTEMPTY)       msg = "Directory not empty";
    S(WSAEPROCLIM)        msg = "Too many processes";
    S(WSAEUSERS)          msg = "Too many users";
    S(WSAEDQUOT)          msg = "Disc quota exceeded";
    S(WSAESTALE)          msg = "Stale NFS file handle";
    S(WSAEREMOTE)         msg = "Too many levels of remote in path";
    S(WSASYSNOTREADY)     msg = "Network system is unavailable";
    S(WSAVERNOTSUPPORTED) msg = "Winsock version out of range";
    S(WSANOTINITIALISED)  msg = "WSAStartup not yet called";
    S(WSAEDISCON)         msg = "Graceful shutdown in progress";
    S(WSAHOST_NOT_FOUND)  msg = "Host not found";
    S(WSANO_DATA)         msg = "No host data of that type was found";
    #undef S

    return msg;
}
#endif

///*
//**perror() equivalent taking to consideration winsock's nil use of errno
//*/
//void dasnethelps_perror(char *prefix)
//{
//  if(!prefix) prefix = NULL;
//#if OS_WINDOWS
//  fprintf(stderr,"%s: %s",prefix,WSAGetLastErrorMessage());
//#else
//  perror(prefix);
//#endif
//}

//#if OS_WINDOWS
////MAKEWORD(1,1) //winsock 1.1
////MAKEWORD(2,0) //winsock 2.0
//int dasnethelps_init()
//{
//  int ret;
//  //WSAData wsaData;
//  WSADATA wsaData;
//  if(WSAStartup(MAKEWORD(1,1),&wsaData) != 0)
//    ret = 1;
//  else
//    ret = 0;
//  return ret;
//}
//int dasnethelps_quit()
//{
//  WSACleanup();
//}
//#endif
