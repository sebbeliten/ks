#NoEnv  ; Recommended for performance and compatibility with future AutoHotkey releases.
; #Warn  ; Enable warnings to assist with detecting common errors.
SendMode Input  ; Recommended for new scripts due to its superior speed and reliability.
SetWorkingDir %A_ScriptDir%  ; Ensures a consistent starting directory.

ToggleWinMinimize(SDL_app)
{
SetTitleMatchMode,2
DetectHiddenWindows, Off
IfWinActive, %SDL_app%
{
WinMinimize, %SDL_app%
}
Else
{
IfWinExist, %SDL_app%
{
WinGet, winid, ID, %SDL_app%
DllCall("SwitchToThisWindow", "UInt", winid, "UInt", 1)
}
}
Return
}

!^k::ToggleWinMinimize("SDL_app")
