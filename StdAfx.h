#if _MSC_VER > 1000
#pragma once
#endif 

#ifndef INCLUDE_MAIN_HEADER_2937733345234234
#define INCLUDE_MAIN_HEADER_2937733345234234

//code does not compile under release mode. only compiles under debug mode. when testing compiles should compile under both modes as the linker/compiler will reivew for different errors (especially if using level 4)
//ERROR: will need to add your version of windows here for the other headers to correctly get params and other
//OS specific items for a server-level component
//should add "#warning" lines to code as needed for compile references

//warning level of DEBUG build needs to be set to 4
//warning level of RELEASE build needs to be set to 4
//release build still have preprocessor DEBUG commands
//release build still includes debug symbols
//object references include more libs thans needed. should review for removal for compiler performance and also not confusing the linker
//review for adding NT EventLog notifications
//readme.txt does not say anything significant. either use this file or remove from project and from harddrive to remove confusion.

//for server components that are run at the ring-0 level this option is rarely used. review this line for removal.
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#define _WIN32_WINNT 0x0400

#include "windows.h"
#include "httpfilt.h"
#include <stdlib.h>
#include "mapbuilder.h"
#include "time.h"
#include "cadobinding.h"
#include "CDataReturn.h"
#include "process.h"
#include "perftimer.h"

//#include "wincrypt.h"
//#include "Httpext.h"
//#include "Base64Coder.h"

//#include "base64.h"
//#include "stringbuilder.h"

#endif