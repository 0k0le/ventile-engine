  /*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
 /*--/ Vulkan API Test Software /--*/   /**/   /*-----/----- Ventile.h ----/-----*/
/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
  /*------------------------/-----------------------------------/-----------------*/
 /*-- Matthew Todd Geiger -/- matthewgeigersoftware@gmail.com -/- 425-363-0746 --*/
/*------------------------/-----------------------------------/-----------------*/

/*
	This file is part of Vulkan Engine Test.

	Vulkan Engine Test is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Vulkan Engine Test is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Vulkan Engine Test.  If not, see <https://www.gnu.org/licenses/>.

	signature of Matthew Geiger 5 June 2020
	Matthew Geiger, Founder of Freespace Software
*/

// DEVELOPMENT INFORMATION
// -------------------
// Vulkan SDK : 1.2.141.0
//				v.w.xxx.0
// -------------------
// nVidia Driver : 450.82
// -------------------
// Windows 10 Home
// Evaluation Copy. Build 19631.mn_release.200514-1410
// -------------------

#pragma once

// DEBUG BUILD FORCED
#ifndef _DEBUG
#define _DEBUG
#endif

// Force windows subsystem
#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")
#pragma comment(linker, "/NODEFAULTLIB:library")
#endif

// Allow unsafe/POSIX functions
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

#ifndef _WIN32
#define _DEFAULT_SOURCE
#endif

#define EXIT_LOOP 0x2

// C++ Headers
// -------------------
#include <iostream>
#include <chrono>

// C Headers
// -------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdarg.h>
#include <sys/timeb.h>

// Windows Headers
// -------------------
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#include <process.h>
#endif

// Linux Header
// -------------------
#include <fcntl.h>
#if !defined(_WIN32)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <strings.h>
#include <stddef.h>
#include <pthread.h>
#include <dirent.h>
#include <bits/stdc++.h>

// KBD Event headers
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

#undef ERR

// Vulkan Headers
// -------------------
#include <vulkan/vulkan.h>


// GLFW Headers
// -------------------
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Typedefinitions and Namespaces
// -------------------
using std::cout;
using std::endl;
using std::cerr;

// MACROS
// -------------------
#define UNUSED(x)((void)(x)) // Ignore compiler warnings : Use rarely

#define LOGFAIL 0
#define LOGSUCCESS 1

#define PERCISIONTIME double

// Set process exit function
#ifdef _WIN32
#define FORCEINLINE __forceinline
#define MOUSERETTYPE LONG
#define KILLKEYPRINTF "%lu"
#define KILLKEYTYPE int
#define SLEEP(x) (Sleep(x))
#define noinline __declspec(noinline)
#define EXIT(exitcode) ExitProcess(exitcode)
#define ERR(format, ...) fprintf(stderr, "WIN --> Fatal error in %s(): " format "\n%s\n", __func__, ##__VA_ARGS__, strerror(errno))
#define ERRL(logger, format, ...) logger->log(LOGFAIL, "WIN --> Fatal error in %s(): " format "\n%s\n", __func__, ##__VA_ARGS__, strerror(errno))
#else
#define FORCEINLINE static __attribute__((always_inline))
#define MOUSERETTYPE unsigned int
#define KILLKEYPRINTF "%u"
#define KILLKEYTYPE unsigned short
#define INPUT_DEVICE_DIRECTORY "/dev/input/by-path/"
#define DEVICE_FILE_DIR_LENGTH 4096
#define SLEEP(x) (usleep(x*1000))
#define noinline __attribute__((noinline))
#define EXIT(exitcode) exit(exitcode)
#define ERR(format, ...) fprintf(stderr, "GNU --> Fatal error in %s(): " format "\n%s\n", __FUNCTION__, ##__VA_ARGS__, strerror(errno))
#define ERRL(logger, format, ...) logger->log(LOGFAIL, "GNU --> Fatal error in %s(): " format "\n%s\n", __FUNCTION__, ##__VA_ARGS__, strerror(errno))
#define MAX_KEY 255

typedef void* (*pthread_func_t)(void*);
#endif

#if !defined(_WIN32)
typedef struct kbd_thread_info {
	pthread_t tid;
	pthread_attr_t attr;
	int kbd_fd;

}kbd_thread_info, * p_kbd_thread_info;

#define MOUSE_EV_OFFSET (BTN_GEAR_UP - BTN_MISC)

typedef struct mouse_state {
	unsigned int button_state[MOUSE_EV_OFFSET];
	unsigned int x_pos, y_pos;
}mouse_state, * p_mouse_state;
#endif

#define BILLION  1E9

// Print error and quit
#define ERRQC(exitcode, format, ...) {\
    ERR(format, ##__VA_ARGS__); \
    fflush(stdout); fflush(stderr); \
	SLEEP(5000); \
    EXIT(exitcode); }

#define ERRQ(format, ...) ERRQC(EXIT_FAILURE, format, ##__VA_ARGS__)

#define ERRLQC(logger, exitcode, format, ...) {\
	if(logger != nullptr) ERRL(logger, format, ##__VA_ARGS__); \
	else ERR(format, ##__VA_ARGS__); \
    fflush(stdout); fflush(stderr); \
	SLEEP(5000); \
    EXIT(exitcode); }

#define ERRLQ(logger, format, ...) ERRLQC(logger, EXIT_FAILURE, format, ##__VA_ARGS__)

// Handle IMPORT/EXPORT MACRO
#ifdef _WIN32
#ifdef _ENGINE_DLL
#define VENTILEAPI __declspec(dllexport)
#else
#define VENTILEAPI __declspec(dllimport)
#endif
#else
// Note: Linux doesnt need dllimport/dllexport or "default"/extern when working with dynamic libs
#define VENTILEAPI
#endif

#ifdef _DEBUG
#define DEBUG(logger, format, ...) { \
	if(logger == nullptr) printf("DEBUG --> %s(): " format "\n", __FUNCTION__, ##__VA_ARGS__); \
	else logger->log(LOGSUCCESS, "DEBUG --> %s(): " format "\n", __FUNCTION__, ##__VA_ARGS__); \
}
#else
#define DEBUG(format, ...)
#endif

// Major.Minor.Release
#define ENGINE_VERSION "0.001.00"

namespace Ventile {
	namespace System {
		// MALLOCS
		// -------------------
		VENTILEAPI void* ec_malloc(const size_t nBytes);

		// I/O
		// -------------------
		VENTILEAPI unsigned int get_file_size(const int fd, const long offset = 0);
		VENTILEAPI int open_file(const char* const file_name, const int flags, char** buf = NULL, const bool isproc = false);

		// Percision Time
		// -------------------
		VENTILEAPI PERCISIONTIME get_percision_time();
	}
}

#include "Application.h"
#ifdef _IS_ENTRY
#include "EntryPoint.h"
#endif