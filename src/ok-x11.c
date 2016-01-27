#include "ok-x11.h"

KeySym OK_TO_XK(int ok)
{
  KeySym keysym;
  #define IS(symbol,xkeysym) if(ok == ok_##symbol) keysym = xkeysym; else
  IS(lshift,XK_Shift_L)
  IS(rshift,XK_Shift_R)
  IS(lctrl,XK_Control_L)
  IS(rctrl,XK_Control_R)
  IS(lalt,XK_Alt_L)
  IS(ralt,XK_Alt_R)
    keysym = 0;
  #undef IS
  return keysym;
}
//#define XK_Shift_L   
//#define XK_Shift_R   
//#define XK_Control_L 
//#define XK_Control_R 
//#define XK_Caps_Lock 
//#define XK_Shift_Lock
//
//#define XK_Meta_L    
//#define XK_Meta_R    
//#define XK_Alt_L     
//#define XK_Alt_R     
//#define XK_Super_L   
//#define XK_Super_R   
//#define XK_Hyper_L   
//#define XK_Hyper_R   
