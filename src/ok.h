#ifndef OTHERKEY_H
#define OTHERKEY_H

/* test if ok is a modifier, and if, return the modifier's ORable number, else, return zero */
int OK_IS_OKMOD(int ok);
//char *OK_TO_STRING(int ok);

/* https://en.wikipedia.org/wiki/Modifier_key#Modifier_keys_on_personal_computers */
#define okmod_null     0x0000
#define okmod_numlock  0x0001
#define okmod_capslock 0x0002 
#define okmod_lctrl    0x0004 
#define okmod_rctrl    0x0008 
#define okmod_lshift   0x0010 
#define okmod_rshift   0x0020 
#define okmod_lalt     0x0040 
#define okmod_ralt     0x0080 
#define okmod_lsuper   0x0100 
#define okmod_rsuper   0x0200 
#define okmod_lmeta    0x0400
#define okmod_rmeta    0x0800
#define okmod_mode     0x1000 //altgr

/* http://www.libsdl.org/release/SDL-1.2.15/docs/html/sdlkey.html */

#define ok_null 0
#define ok_clear 1
#define ok_pause 2
#define ok_kp0 3
#define ok_kp1 4
#define ok_kp2 5
#define ok_kp3 6
#define ok_kp4 7
#define ok_kp5 8
#define ok_kp6 9
#define ok_kp7 10
#define ok_kp8 11
#define ok_kp9 12
#define ok_up 13
#define ok_down 14
#define ok_right 15
#define ok_left 16
#define ok_insert 17
#define ok_home 18
#define ok_end 19
#define ok_pageup 20
#define ok_pagedown 21
#define ok_f1 22
#define ok_f2 23
#define ok_f3 24
#define ok_f4 25
#define ok_f5 26
#define ok_f6 27
#define ok_f7 28
#define ok_f8 29
#define ok_f9 30
#define ok_f10 31
#define ok_f11 32
#define ok_f12 33
#define ok_f13 34
#define ok_f14 35
#define ok_f15 36
#define ok_numlock 37
#define ok_capslock 38
#define ok_scrollock 39
#define ok_rshift 40
#define ok_lshift 41
#define ok_rctrl 42
#define ok_lctrl 43
#define ok_ralt 44
#define ok_lalt 45
#define ok_rmeta 46
#define ok_lmeta 47
#define ok_lsuper 48
#define ok_rsuper 49
#define ok_mode 50
#define ok_help 51
#define ok_print 52
#define ok_sysreq 53
#define ok_break 54
#define ok_menu 55
#define ok_power 56
#define ok_euro 57
#define ok_counted 58

#endif
