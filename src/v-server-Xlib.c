
#include "miscellaneous/msleep.h"

#define BACKLOG 10
#include "dasnethelp.h"

#include <errno.h>
#define perror_getaddrinfo(getaddrinfo_return_value) \
  { fprintf(stderr,"getaddrinfo: %s\n",gai_strerror(getaddrinfo_return_value)); exit(1); }
#define perrno(string) \
  { dasnethelps_perror(string); exit(1); }
#define errxit(string) \
  { fprintf(stderr,string"\n"); exit(1); }

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysymdef.h>
#include <X11/extensions/XTest.h>

//#include "ok-x11.h"

#include <stdbool.h>
char *opt_port = "1112";
char *opt_iface = "eth0";

//#include "macroopt.h"
int main(int argc, char *argv[])
{
  Display *dsp = XOpenDisplay(0);
  if(!dsp) {
    fprintf(stderr,"XOpenDisplay(): was unable to open\n");
    exit(1);
  }
  int screen_number = DefaultScreen(dsp);

  /*  XXX: sending the pixels of the root window
           to the client
  
      http://cgit.freedesktop.org/xorg/app/xwd/tree/xwd.c
      $ man XGetImage

      there's a field called “data” in the XImage
      structure.
      $ vim +365 /usr/include/X11/Xlib.h

      all that remains is to copy those pixels
      and send them to the client but, that involves
      changing the code to use a non blocking client socket
      and, creating an fps setting ... don't need non-blocking,
      simply need to sleep per frame to the next frame, send
      copy and send pixels, repeat, separate executable, easier
  */
  Window root_window = RootWindow(dsp,screen_number);
  XWindowAttributes attr;
  if(!XGetWindowAttributes(dsp,root_window,&attr))
    errxit("XGetWindowAttributes(): error");

  //printf("Width: %d\n"
  //       "Height: %d\n",attr.width,attr.height);
  //XImage *image = XGetImage(dsp,root_window,
  //                          attr.x,attr.y,
  //                          attr.width,attr.height,
  //                          AllPlanes,XYPixmap);

  /* XXX: warping the pointer using offsets gotten from
          the client 

          $ man XWarpPointer
  */
  //int XWarpPointer(Display *display, Window src_w, Window dest_w,
  //       int src_x, int src_y, unsigned int src_width, unsigned
  //       int src_height, int dest_x, int dest_y);
  //XWarpPointer(dsp,None,None,
  //             0,0,0,0,
  //             pointer_offset_x,
  //             pointer_offset_y);

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
  
  char listener_ipaddr_buf[INET6_ADDRSTRLEN];
  int listener_port;
  
  listener_port = sockaddr_inX_to_presentable(res->ai_family,&addr,listener_ipaddr_buf);

  while(1)
  {
      
    printf("\nCONNECT TO THIS ADDRESS AND PORT:\n\n\t%s\n\t%d\n\n\n",listener_ipaddr_buf,listener_port);

    if(listen(listener,BACKLOG)) { perrno("listen()"); }

    typedef struct { int socket; struct sockaddr addr; socklen_t addrlen; }client_t;
    client_t client;
    client.addrlen = sizeof(client.addr);

    if((client.socket = accept(listener,&client.addr,&addrlen)) < 0) {
      perrno("accept()");
    }

    char client_ipaddr_buf[INET6_ADDRSTRLEN];
    int client_port;

    //res->ai_family does not reflect client's IP version
    client_port = sockaddr_inX_to_presentable(res->ai_family,&client.addr,client_ipaddr_buf);

    printf("ACCEPTED CLIENT\n\n\t%s\n\t%d\n\n\n",client_ipaddr_buf,client_port);
    
    // begin send() copies of the pixels on the screen
    // int32_t ;; width
    // int32_t ;; height
    // int32_t ;; bits per pixel
    // int32_t ;; mask red
    // int32_t ;; mask green
    // int32_t ;; mask blue
    // int32_t ;; mask alpha ;; don't know if it is needed
    // ??????? ;; pixels

    // TODO: need sleep call
    // nanosleep, sleep ;; POSIX

    XImage *image = XGetImage(dsp,root_window,
                              attr.x,attr.y,
                              attr.width,attr.height,
                              AllPlanes,XYPixmap);
    int32_t     width = 0
    ,          height = 0
    , bits_per_pixels = 0
    ,        mask_red = 0
    ,      mask_green = 0
    ,       mask_blue = 0
    ,      mask_alpha = 0
    ;
    send(client.socket
    
    msleep(1000 / 60);
  }
  XCloseDisplay(dsp);
  return 0;
}








