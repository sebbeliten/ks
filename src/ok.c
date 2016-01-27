#include "ok.h"

/* test if ok is a modifier, and if, return the modifier's ORable number, else, return zero */
int OK_IS_OKMOD(int ok)
{
  #define OKMOD(SYMBOL) if( ok == ok_##SYMBOL ) return okmod_##SYMBOL;
  OKMOD(lshift);
  OKMOD(rshift);
  OKMOD(lctrl);
  OKMOD(rctrl);
  OKMOD(lsuper);
  OKMOD(rsuper);
  OKMOD(lalt);
  OKMOD(ralt);
  OKMOD(lmeta);
  OKMOD(rmeta);
  OKMOD(capslock);
  OKMOD(numlock);
  OKMOD(mode);
  #undef OKMOD
  return okmod_null;
}

char *OK_TO_STRING(int ok)
{
  char *string;
  #define symbol_to_string(ok,symbol) if(ok == ok_##symbol) string = #symbol ; else
  symbol_to_string(ok,null)
  symbol_to_string(ok,clear)
  symbol_to_string(ok,pause)
  symbol_to_string(ok,kp0)
  symbol_to_string(ok,kp1)
  symbol_to_string(ok,kp2)
  symbol_to_string(ok,kp3)
  symbol_to_string(ok,kp4)
  symbol_to_string(ok,kp5)
  symbol_to_string(ok,kp6)
  symbol_to_string(ok,kp7)
  symbol_to_string(ok,kp8)
  symbol_to_string(ok,kp9)
  symbol_to_string(ok,up)
  symbol_to_string(ok,down)
  symbol_to_string(ok,right)
  symbol_to_string(ok,left)
  symbol_to_string(ok,insert)
  symbol_to_string(ok,home)
  symbol_to_string(ok,end)
  symbol_to_string(ok,pageup)
  symbol_to_string(ok,pagedown)
  symbol_to_string(ok,f1)
  symbol_to_string(ok,f2)
  symbol_to_string(ok,f3)
  symbol_to_string(ok,f4)
  symbol_to_string(ok,f5)
  symbol_to_string(ok,f6)
  symbol_to_string(ok,f7)
  symbol_to_string(ok,f8)
  symbol_to_string(ok,f9)
  symbol_to_string(ok,f10)
  symbol_to_string(ok,f11)
  symbol_to_string(ok,f12)
  symbol_to_string(ok,f13)
  symbol_to_string(ok,f14)
  symbol_to_string(ok,f15)
  symbol_to_string(ok,numlock)
  symbol_to_string(ok,capslock)
  symbol_to_string(ok,scrollock)
  symbol_to_string(ok,rshift)
  symbol_to_string(ok,lshift)
  symbol_to_string(ok,rctrl)
  symbol_to_string(ok,lctrl)
  symbol_to_string(ok,ralt)
  symbol_to_string(ok,lalt)
  symbol_to_string(ok,rmeta)
  symbol_to_string(ok,lmeta)
  symbol_to_string(ok,lsuper)
  symbol_to_string(ok,rsuper)
  symbol_to_string(ok,mode)
  symbol_to_string(ok,help)
  symbol_to_string(ok,print)
  symbol_to_string(ok,sysreq)
  symbol_to_string(ok,break)
  symbol_to_string(ok,menu)
  symbol_to_string(ok,power)
  symbol_to_string(ok,euro)
    string = (void*)0;
  #undef symbol_to_string
  return string;
}
