//http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system

#ifndef WHICHOS_H
#define WHICHOS_H

#if defined(_WIN32) || defined(_WIN64)
  #define OS_WINDOWS 
#endif

/* this symbol must be passed in by the user */
#ifdef OS_WINDOWS_XP
  #ifndef OS_WINDOWS
    #error "cannot compile for windows XP when the operating system isn't windows"
  #else
    #define _WIN32_WINNT 0x501
    //#define WINVER 0x501
  #endif
#endif

#endif
