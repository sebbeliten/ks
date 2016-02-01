#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

;the problem is that synergy cannot detect remapped keys with shift in them
;so (atleast to my knowledge about autohotkey) i must limit my remapping
;to 
;
;
;not using SendInput, ...
;causes garbage output like: <9b> for /, <9d> for =
;using it, causes the same output to occur
;

;
;on synergy client machine
;
;using shift (+) on left, causes: <9b> for /, <9d> for =
;
;using ditto, causes the regularly bound key, eg, " on 2 (if 2 had been bound to ")
;
;i must see if i can avoid specifying the shift key, rather,
;directly specify the key, eg, @ for (pseudocode) 55, instead of +2
;
;

; http://superuser.com/questions/749929/how-can-i-re-map-the-colon-shiftperiod
; remap Colon to Period

;SWAP COLON PERIOD
#UseHook, On
:::SendInput, . ; Oddly enough, a colon as a hotkey trigger doesn't need to be escaped, but a colon in a hotstring, unescaped or otherwise, just breaks things. Wouldn't get triggered by :, even if uncommented (that's good!)
Return
#UseHook, Off
.:::
Return
;SWAP SEMICOLON COMMA
+,::SendInput, ,
Return
,::;
Return

;SWAP SYMBOL-ROW NUMBER-ROW
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


;å::=
;Return

;\::/
;Return
;æ::,
;Return
;'::.
;Return

;ive got to learn to use the left shift instead of the right 
;shift instead!
;æ::Shift
;Return
