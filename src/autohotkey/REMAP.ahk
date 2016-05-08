#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

;
;https://www.autohotkey.com/docs/Tutorial.htm
;

;; period is colon
#UseHook, On
:::SendInput, .
Return
#UseHook, Off
;; colon is period
.:::
Return

;; comma is semicolon
+,::SendInput, ,
Return
;; semicolon is comma
,::;
Return

;; altgr as an alternative to shift
<^>!7::SendInput, /
Return
<^>!.::SendInput, .
Return
<^>!,::SendInput, ,
Return
<^>!_::SendRaw, _
Return

;; the number row will yield its equivalent symbols
1::!
Return
+2::SendInput, @
Return
2::"
Return
3::#
Return
4::$
Return
;%
5::+5
Return
6::&
Return
+7::/
Return
7::{
Return
+8::SendInput, [
Return
8::(
Return
+9::SendInput, ]
Return
9::)
Return
+0::=
Return
0::}
Return

;; like C-m is enter, like C-h is backspace
<^>!h::Backspace
Return
<^>!m::Enter
Return

;; send altgr when lowercase Æ is pressed, doesn't work
;æ::
;SendInput, {LCtrl Down}{RAlt Down}
;KeyWait, æ
;SendInput, {LCtrl Up}{RAlt Up}
;Return

;; altgr on the row below the number row will yield the number row
<^>!u::SendInput, 7
<^>!i::SendInput, 8
<^>!o::SendInput, 9

<^>!r::SendInput, 4
<^>!t::SendInput, 5
<^>!y::SendInput, 6

<^>!q::SendInput, 1
<^>!w::SendInput, 2
<^>!e::SendInput, 3

<^>!p::SendInput, 0

;; caps lock is escape
CapsLock::Escape
