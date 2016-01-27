#!/bin/sh

s="
#define ok_null
#define ok_clear
#define ok_pause
#define ok_kp0
#define ok_kp1
#define ok_kp2
#define ok_kp3
#define ok_kp4
#define ok_kp5
#define ok_kp6
#define ok_kp7
#define ok_kp8
#define ok_kp9
#define ok_up
#define ok_down
#define ok_right
#define ok_left
#define ok_insert
#define ok_home
#define ok_end
#define ok_pageup
#define ok_pagedown
#define ok_f1
#define ok_f2
#define ok_f3
#define ok_f4
#define ok_f5
#define ok_f6
#define ok_f7
#define ok_f8
#define ok_f9
#define ok_f10
#define ok_f11
#define ok_f12
#define ok_f13
#define ok_f14
#define ok_f15
#define ok_numlock
#define ok_capslock
#define ok_scrollock
#define ok_rshift
#define ok_lshift
#define ok_rctrl
#define ok_lctrl
#define ok_ralt
#define ok_lalt
#define ok_rmeta
#define ok_lmeta
#define ok_lsuper
#define ok_rsuper
#define ok_mode
#define ok_help
#define ok_print
#define ok_sysreq
#define ok_break
#define ok_menu
#define ok_power
#define ok_euro
"

IFS='
'

n=0
for e in $s
do
  echo $e $n
  n=$((n+1))
done
