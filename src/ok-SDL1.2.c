#include "ok-SDL1.2.h"

/* return ok_* if SDLKey (ok) is ok_* */
int SDLKey_TO_OK(int ok)
{
  #define ISOK(sdlk_xxx,N) if(sdlk_xxx == ok) ok = N; else
  ISOK(SDLK_CLEAR,1)
  ISOK(SDLK_PAUSE,2)
  ISOK(SDLK_KP0,3)
  ISOK(SDLK_KP1,4)
  ISOK(SDLK_KP2,5)
  ISOK(SDLK_KP3,6)
  ISOK(SDLK_KP4,7)
  ISOK(SDLK_KP5,8)
  ISOK(SDLK_KP6,9)
  ISOK(SDLK_KP7,10)
  ISOK(SDLK_KP8,11)
  ISOK(SDLK_KP9,12)
  ISOK(SDLK_UP,13)
  ISOK(SDLK_DOWN,14)
  ISOK(SDLK_RIGHT,15)
  ISOK(SDLK_LEFT,16)
  ISOK(SDLK_INSERT,17)
  ISOK(SDLK_HOME,18)
  ISOK(SDLK_END,19)
  ISOK(SDLK_PAGEUP,20)
  ISOK(SDLK_PAGEDOWN,21)
  ISOK(SDLK_F1,22)
  ISOK(SDLK_F2,23)
  ISOK(SDLK_F3,24)
  ISOK(SDLK_F4,25)
  ISOK(SDLK_F5,26)
  ISOK(SDLK_F6,27)
  ISOK(SDLK_F7,28)
  ISOK(SDLK_F8,29)
  ISOK(SDLK_F9,30)
  ISOK(SDLK_F10,31)
  ISOK(SDLK_F11,32)
  ISOK(SDLK_F12,33)
  ISOK(SDLK_F13,34)
  ISOK(SDLK_F14,35)
  ISOK(SDLK_F15,36)
  ISOK(SDLK_NUMLOCK,37)
  ISOK(SDLK_CAPSLOCK,38)
  ISOK(SDLK_SCROLLOCK,39)
  ISOK(SDLK_RSHIFT,40)
  ISOK(SDLK_LSHIFT,41)
  ISOK(SDLK_RCTRL,42)
  ISOK(SDLK_LCTRL,43)
  ISOK(SDLK_RALT,44)
  ISOK(SDLK_LALT,45)
  ISOK(SDLK_RMETA,46)
  ISOK(SDLK_LMETA,47)
  ISOK(SDLK_LSUPER,48)
  ISOK(SDLK_RSUPER,49)
  ISOK(SDLK_MODE,50)
  ISOK(SDLK_HELP,51)
  ISOK(SDLK_PRINT,52)
  ISOK(SDLK_SYSREQ,53)
  ISOK(SDLK_BREAK,54)
  ISOK(SDLK_MENU,55)
  ISOK(SDLK_POWER,56)
  ISOK(SDLK_EURO,57)
    ok = ok_null;
  #undef ISOK
  return ok;
}