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

// Force windows subsystem
#ifdef _WIN32
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

// Allow unsafe/POSIX functions
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#endif

// C++ Headers
// -------------------
#include <iostream>

// C Headers
// -------------------
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

// Windows Headers
// -------------------
#ifdef _WIN32
#include <Windows.h>
#include <io.h>
#endif

// Linux Header
// -------------------
#include <fcntl.h>
#if !defined(_WIN32)
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <strings.h>
#include <stddef.h>
#include <pthread.h>
#include <dirent.h>

// KBD Event headers
#include <linux/input.h>
#include <linux/input-event-codes.h>
#endif

#undef ERR

// Vulkan Headers
// -------------------

// Typedefinitions and Namespaces
// -------------------
using std::cout;
using std::endl;
using std::cerr;

// MACROS
// -------------------
#define UNUSED(x)((void)(x)) // Ignore compiler warnings : Use rarely

// Set process exit function
#ifdef _WIN32
#define SLEEP(x) (Sleep(x))
#define noinline __declspec(noinline)
#define EXIT(exitcode) ExitProcess(exitcode)
#define ERR(format, ...) fprintf(stderr, "WIN --> Fatal error in %s(): " format "\n%s\n", __func__, ##__VA_ARGS__, strerror(errno))
#else
#define INPUT_DEVICE_DIRECTORY "/dev/input/by-path/"
#define DEVICE_FILE_DIR_LENGTH 4096
#define SLEEP(x) (sleep(x/1000))
#define noinline __attribute__((noinline))
#define EXIT(exitcode) exit(exitcode)
#define ERR(format, ...) fprintf(stderr, "GNU --> Fatal error in %s(): " format "\n%s\n", __FUNCTION__, ##__VA_ARGS__, strerror(errno))
#define MAX_KEY 255

typedef void* (*pthread_func_t)(void*);
#endif

#if !defined(_WIN32)
typedef struct kbd_thread_info {
	pthread_t tid;
	pthread_attr_t attr;
	int kbd_fd;

}kbd_thread_info, * p_kbd_thread_info;
#endif

// Print error and quit
#define ERRQC(exitcode, format, ...) {\
    ERR(format, ##__VA_ARGS__); \
    fflush(stdout); fflush(stderr); \
    EXIT(exitcode); }

#define ERRQ(format, ...) ERRQC(EXIT_FAILURE, format, ##__VA_ARGS__)

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

// Major.Minor.Release
#define ENGINE_VERSION "0.001.00"

#include "Application.h"
#include "EntryPoint.h"