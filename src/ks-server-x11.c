
#define BACKLOG 10
#include "dasnethelp.h"

#include <errno.h>
#define perror_getaddrinfo(getaddrinfo_return_value) \
  { fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(getaddrinfo_return_value)); exit(1); }
#define perrno(string) \
  { dasnethelps_perror(string); exit(1); }

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

#include "ok-x11.h"

char *opt_port = "1111";
char *opt_iface = "eth0";

//#include "macroopt.h"
int main(int argc, char *argv[])
{
  Display *dsp = XOpenDisplay(0);
  if(!dsp) {
    fprintf(stderr,"XOpenDisplay(): was unable to open\n");
    exit(1);
  }
  int screen = DefaultScreen(dsp);

  struct addrinfo hints, *res;

  memset(&hints,0,sizeof hints);

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  char *ifaceaddr = getifaceaddr(opt_iface);
  if(!ifaceaddr) {
    fprintf(stderr,"getifaceaddr(): interface `%s' isn't in existence\n",opt_iface); exit(1);
  }

  { int ERROR;
    if(ERROR=getaddrinfo(ifaceaddr,opt_port,&hints,&res))
      perror_getaddrinfo(ERROR);
  }

  int listener = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
  if(listener < 0) {
    perrno("socket()");
  }

  if(bind(listener,res->ai_addr,res->ai_addrlen)) {
    perrno("bind()");
  }

  struct sockaddr addr;
  int addrlen = sizeof addr;
  if(getsockname(listener,&addr,&addrlen)) {
    perrno("getsockname()");
  }
  
  char ipaddr_buf[INET6_ADDRSTRLEN];
  int port;
  
  port = sockaddr_inX_to_presentable(res->ai_family,&addr,ipaddr_buf);
  
  printf("\nCONNECT TO THIS ADDRESS AND PORT:\n\n\t%s\n\t%d\n\n\n",ipaddr_buf,port);

  if(listen(listener,BACKLOG)) { perrno("listen()"); }

  typedef struct { int socket; struct sockaddr addr; socklen_t addrlen; }client_t;
  client_t client;
  client.addrlen = sizeof(client.addr);

  if((client.socket = accept(listener,&client.addr,&addrlen)) < 0) {
    perrno("accept()");
  }

  port = sockaddr_inX_to_presentable(res->ai_family,&client.addr,ipaddr_buf);

  printf("ACCEPTED CLIENT\n\n\t%s\n\t%d\n\n\n",ipaddr_buf,port);

  size_t count = sizeof(int32_t) * 3;
  char buffer[count];
  while(1)
  {
    size_t counted = recv(client.socket,buffer,count,0);
    if(counted < 0) {
      perrno("read()");
    }
    else
    if(counted > 0)
    {
      int32_t *num = (int32_t*)buffer;
      int32_t release = ntohl(num[0]),
                   ok = ntohl(num[1]),
                   ch = ntohl(num[2]);
      //Bool pressed = m.pressed ? True:False;
      Bool pressed = release ? False:True;
      KeyCode keycode;

      if(ok == 0)
      {
        if(ch >= 0x0008 && ch <= 0x000d)
          ch += 0xff00;
        keycode = XKeysymToKeycode(dsp,ch);
      }
      else
      {
        //KeySym s;
        //printf("ok is here: %d\n",ok);
        //if(ok == ok_rshift)
        //  s = XK_Shift_R;
        //else
        //  continue;
        KeySym s;
        s = OK_TO_XK(ok);
        if(!s)
          continue;
        //if(s == XK_Shift_R)
        //  puts("shift pressed");
        //puts("yeah");
        keycode = XKeysymToKeycode(dsp,s);
      }

      XTestFakeKeyEvent(dsp,keycode,pressed,CurrentTime);
      XFlush(dsp);
    }
  }
  XCloseDisplay(dsp);
  return 0;
}








