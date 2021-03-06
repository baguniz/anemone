/**
* This file is part of Anemone.
*
* Anemone is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* The Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Anemone is distributed in the hope that it will be useful,
*
* But WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Anemone.
*
*   If not, see <http://www.gnu.org/licenses/>.
*
**/


#pragma once
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN

#define ANEMONE_VER "Anemone v2.0"

#pragma comment(linker,"\"/manifestdependency:type='win32' name = 'Microsoft.Windows.Common-Controls' version = '6.0.0.0' processorArchitecture = '*' publicKeyToken = '6595b64144ccf1df' language = '*'\"")

// Windows Header
#include <windows.h>
#include <process.h>
#include <commdlg.h>
#include <commctrl.h>

// disable 4819 charset warnings
#pragma warning(disable: 4819)

// skia-compatiblity
#undef max
#undef min

// c runtime
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// c++ runtime
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>

// fmt
#include <fmt/format.h>

extern int guide_opacity;
extern bool mouse_evt;
extern int text_speed;

#include "ConfigManager.h"
extern ConfigManager CfgMgr;

#define _TXT(s) #s
#define TXT(s) _TXT(s)
#define WIDEN(quote) WIDEN2(quote)
#define WIDEN2(quote) L##quote

extern WCHAR g_Version[100];
extern WCHAR g_BuildInfo[100];
extern WCHAR g_AnemoneInfo[100];