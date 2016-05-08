

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

#include "ok-Xlib.h"

#include <stdbool.h>
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
  int screen_number = DefaultScreen(dsp);

  /*
      a server listen for incoming connection
      a client initiate connection

      the specification is so that
      there is one socket connection
      per device and whether connection
      is spread out through different
      executable make no difference

      backend ;; sdl,x11,etc

           -k ;; socket connection
           -v ;; socket connection
           -m ;; socket connection

      kvm-client-sdl [-k ADDRESS -K PORT] [-v ADDRESS -V PORT] [-m ADDRESS -M PORT]
      kvm-client-sdl -kvm ADDRESS
      
      k-server-x11 
      v-server-x11
      m-server-x11

      kvm-server-x11
      kvm-server-x11
      
      and, replacing server/client with receiver/sender.

      let's be plain.
      k-sender ;; sends keyboard to k-receiver
      v-sender ;; sends    video to v-receiver
      m-sender ;; sends    mouse to m-receiver

      a kvm-*receiver* -v is a *sender*
      ignore
  */

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
  //XWindowAttributes attr;
  //if(!XGetWindowAttributes(dsp,root_window,&attr))
  //  errxit("XGetWindowAttributes(): error");

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

    //temporary solution to remapping number row: i do this in autohotkey instead
    //int is_altgr = false;

    size_t count = sizeof(int32_t) * 3;
    char buffer[count];
    while(1)
    {
      size_t counted = recv(client.socket,buffer,count,0);
      if(counted < 0) {
        perrno("read()");
      }else
      if(!counted)
      {
        /* client exited -- should wait for a new connection */
        puts("DISCONNECTED");
        close(client.socket);
        break;
      }else
      if(counted > 0)
      {
        int32_t *num = (int32_t*)buffer;
        int32_t release = ntohl(num[0]),
                     ok = ntohl(num[1]),
                     ch = ntohl(num[2]);
        //Bool pressed = m.pressed ? True:False;
        Bool pressed = release ? False:True;
        KeyCode keycode;
        //int altgr_try = false;

        //printf("UNHANDLED: %d\n",ch);
        /* it's an ascii value */
        if(ok == 0)
        {
          if(ch < 32 || ch == 127)
          {
            //keysyms below 32 might not be calculated as below
            //#define XK_BackSpace                     0xff08  /* Back space, back char */
            //#define XK_Tab                           0xff09
            //#define XK_Linefeed                      0xff0a  /* Linefeed, LF */
            //#define XK_Clear                         0xff0b
            //#define XK_Return                        0xff0d  /* Return, enter */
            //#define XK_Pause                         0xff13  /* Pause, hold */
            //#define XK_Scroll_Lock                   0xff14
            //#define XK_Sys_Req                       0xff15
            //#define XK_Escape                        0xff1b
            //#define XK_Delete                        0xffff  /* Delete, rubout */
            #define ITS(ASCII,SYMBOL) case ASCII: ch = SYMBOL; break
            switch(ch)
            {
              ITS(8,XK_BackSpace);
              ITS(9,XK_Tab);
              ITS(10,XK_Linefeed);
              //ITS(11,XK_VerticalTab);
              //ITS(12,XK_FormFeed);
              ITS(13,XK_Return);
              ITS(27,XK_Escape);
              ITS(127,XK_Delete);
              default:
                fprintf(stderr,"UNHANDLED: %d\n",ch);
                continue;
            }
            #undef ITS
            //else
            //  continue;
          }
          /* the value is not a control character */
          else
          {
            // receive us qwerty, convert to no qwerty
            // might be more convenient to do the conversion on the client
            if(ch == ';')
              ch = XK_oslash;
            else
            if(ch == '/')
              ch = XK_minus;
            else
            if(ch == '\\')
              ch = XK_quoteright;
            else
            if(ch == '=')
              ch = XK_backslash;
            else
            if(ch == '-')
              ch = XK_plus;
            else
            if(ch == ']')
              ch = XK_dead_diaeresis;
            else
            if(ch == '[')
              ch = XK_aring;
            else
            if(ch == '\'')
              ch = XK_ae;
            else
            if(ch == '`')
              ch = XK_bar;
            
            //remap
            //something more complex would have to be done to get keypress rate to work
            //else
            //if(ch == 'q' && is_altgr == True)
            //{
            //  //printf("is_altgr: %d\n",is_altgr);
            //  ch = XK_1;
            //  XTestFakeKeyEvent(dsp,XKeysymToKeycode(dsp,XK_ISO_Level3_Shift),False,CurrentTime);
            //  XTestFakeKeyEvent(dsp,XKeysymToKeycode(dsp,ch),pressed,CurrentTime);
            //  XTestFakeKeyEvent(dsp,XKeysymToKeycode(dsp,XK_ISO_Level3_Shift),True,CurrentTime);
            //  //s = XK_ISO_Level3_Shift;
            //  XFlush(dsp);
            //  continue;
            //}
            //dead_diaeresis
            //else
            //  ;
            //if(ch >= 0x0008 && ch <= 0x000d)
            //  ch += 0xff00;
            //else
            //{
            //  printf("UNHANDLED: %d\n",ch);
            //  continue;
            //}
          }
          
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
          //temporary solution to generating altgr cuz sdl sends lctrl
          //and ralt in sequence instead of it
          //if(ok == ok_lctrl && false == altgr_try)
          //{
          //  altgr_try = true;
          //}
          //else
          //if(altgr_try == true)
          //{
          //  if(ok == ok_ralt)
          //  {
          //    //#define XK_ISO_Level3_Shift              0xfe03
          //    s = XK_ISO_Level3_Shift;
          //  }
          //  else
          //  {
          //    s = XK_Control_L;
          //  }
          //  keycode = XKeysymToKeycode(dsp,s);
          //  XTestFakeKeyEvent(dsp,keycode,pressed,CurrentTime);
          //  
          //  altgr_try = false;
          //}
          //else
          /* but wait a minute... there's no ralt!! */
          /* this should maybe be handled in the client */
          if(ok == ok_ralt)
          {
            //unpress lctrl
            XTestFakeKeyEvent(dsp,XKeysymToKeycode(dsp,XK_Control_L),False,CurrentTime);
            XFlush(dsp);
            s = XK_ISO_Level3_Shift;
            //is_altgr = pressed;
            //printf("is_altgr: %d\n",is_altgr);
          }
          //hello i am really not so hungry right now though!^H^H^HUNHANDLED: 0
          /* did not work well */
          //else
          //if(ok == ok_capslock)
          //  s = XK_Escape;
          /* convert the OK value to a corresponding X keysym*/
          else
          {
            //s = OK_TO_XK(ok);
            #define ITS(OKSYMBOL,XSYMBOL) if(ok == ok_##OKSYMBOL) s = XSYMBOL; else
            // MODIFIERS
            ITS(lshift,XK_Shift_L)
            ITS(rshift,XK_Shift_R)
            ITS(lctrl,XK_Control_L)
            ITS(rctrl,XK_Control_R)
            ITS(lalt,XK_Alt_L)
            // KEYPAD
            ITS(kp0,XK_0)
            ITS(kp1,XK_1)
            ITS(kp2,XK_2)
            ITS(kp3,XK_3)
            ITS(kp4,XK_4)
            ITS(kp5,XK_5)
            ITS(kp6,XK_6)
            ITS(kp7,XK_7)
            ITS(kp8,XK_8)
            ITS(kp9,XK_9)
            // FUNCTION KEYS
            ITS(f1,XK_F1)
            ITS(f2,XK_F2)
            ITS(f3,XK_F3)
            ITS(f4,XK_F4)
            ITS(f5,XK_F5)
            ITS(f6,XK_F6)
            ITS(f7,XK_F7)
            ITS(f8,XK_F8)
            ITS(f9,XK_F9)
            ITS(f10,XK_F10)
            ITS(f11,XK_F11)
            ITS(f12,XK_F12)
            // MISC
            ITS(home,XK_Home)
            ITS(pageup,XK_Page_Up)
            ITS(pagedown,XK_Page_Down)
            ITS(home,XK_End)
            ITS(left,XK_Left)
            ITS(up,XK_Up)
            ITS(down,XK_Down)
            ITS(right,XK_Right)
            //ITS(ralt,XK_Alt_R)
              s = 0;
            #undef ITS
            if(!s)
              continue;
          }
          keycode = XKeysymToKeycode(dsp,s);
        }

        //man XRefreshKeyboardMapping
        XTestFakeKeyEvent(dsp,keycode,pressed,CurrentTime);
        XFlush(dsp);
      }
    }
  }
  XCloseDisplay(dsp);
  return 0;
}








