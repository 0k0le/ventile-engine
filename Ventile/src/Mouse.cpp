/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/---- Mouse.cpp -----/-----*/
/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
  /*------------------------/-----------------------------------/-----------------*/
 /*-- Matthew Todd Geiger -/- matthewgeigersoftware@gmail.com -/- 425-363-0746 --*/
/*------------------------/-----------------------------------/-----------------*/

/*
	This file is part of Ventrile.

	Ventrile is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Ventrile is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Ventrile.  If not, see <https://www.gnu.org/licenses/>.

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

#include "Ventile.h"

namespace Ventile {
	extern System::Logger* logger;

	namespace System {
#ifndef _WIN32
		void Mouse::get_mouse_event(Mouse* mouse) {
			static input_event ie;
			memset(&ie, 0, sizeof(ie));

			if (read(mouse->fd, &ie, sizeof(ie)) == -1) {
				if (errno != EAGAIN)
					ERRLQ(logger, "Failed to read mse device file!");

				return;
			}

			// Mouse movement
			if (ie.type == EV_ABS) {
				if (ie.code == ABS_X) {
					pthread_mutex_lock(&mouse->mouse_state_mutex);
					mouse->ms.x_pos = ie.value;
					pthread_mutex_unlock(&mouse->mouse_state_mutex);
				}
				else if (ie.code == ABS_Y) {
					pthread_mutex_lock(&mouse->mouse_state_mutex);
					mouse->ms.y_pos = ie.value;
					pthread_mutex_unlock(&mouse->mouse_state_mutex);
				}
			}
			else if (ie.type == EV_KEY) {
				pthread_mutex_lock(&mouse->mouse_state_mutex);
				mouse->ms.button_state[ie.code - MOUSE_EV_OFFSET] = ie.value;
				pthread_mutex_unlock(&mouse->mouse_state_mutex);
			}

			return;
		}

		void Mouse::mouse_thread_signal_handler(int sig) {
			if (sig == SIGSEGV)
				pthread_exit(NULL);
		}

		void* Mouse::mouse_state_thread(void* data) {
			Mouse* mouse = (Mouse*)data;

			signal(SIGSEGV, mouse->mouse_thread_signal_handler);

			while (1) {
				mouse->get_mouse_event(mouse);
			}
		}
		char* Mouse::get_default_mouse_device_file() {
			static char device_file[DEVICE_FILE_DIR_LENGTH] = { 0 };
			DIR* dp = NULL;
			struct dirent* ep = NULL;

			memset(device_file, 0, DEVICE_FILE_DIR_LENGTH);

			if ((dp = opendir(INPUT_DEVICE_DIRECTORY)) == NULL)
				ERRLQ(logger, "Failed to open directory: " INPUT_DEVICE_DIRECTORY "\n");

			while ((ep = readdir(dp))) {
				if (strstr(ep->d_name, "mouse") && strstr(ep->d_name, "pci") == NULL && strlen(ep->d_name) < DEVICE_FILE_DIR_LENGTH) {
					sprintf(device_file, INPUT_DEVICE_DIRECTORY "%s", ep->d_name);
					break;
				}
			}

			closedir(dp);

			DEBUG(logger, "Device File \"%s\"", device_file);

			return device_file;
		}

		int Mouse::open_mse(const char* mse_dev) {
			int fd = 0;

			// Read only
			if ((fd = open(mse_dev, O_RDONLY | O_NOCTTY)) == -1)
				ERRLQ(logger, "Failed to open mouse device!");

			// Non blocking
			int flags = fcntl(fd, F_GETFL, 0);
			if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
				ERRLQ(logger, "Failed to enable non-blocking!");

			DEBUG(logger, "Opened device file \"%s\" successfully!", mse_dev);

			return fd;
		}

		void Mouse::print_input_event(struct input_event* ie) {
			char timebuf[64] = { 0 };
			struct tm* ptm = NULL;

			// Convert timeval to string
			ptm = localtime(&(ie->time.tv_sec));
			strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", ptm);

			printf("INPUT EVENT\n");
			printf("--------------------\n");
			printf("TIME STAMP\n");
			puts(timebuf);
			printf("--------------------\n");
			printf("Type:  %hu -- 0x%04x\n", ie->type, ie->type);
			printf("Code:  %hu -- 0x%04x\n", ie->code, ie->code);
			printf("Value: %d -- 0x%08x\n\n", ie->value, ie->value);
		}

		unsigned int Mouse::get_mouse_x() {
			register unsigned int ret = 0;

			pthread_mutex_lock(&mouse_state_mutex);
			ret = ms.x_pos;
			pthread_mutex_unlock(&mouse_state_mutex);

			return ret;
		}
		unsigned int Mouse::get_mouse_y() {
			register unsigned int ret = 0;

			pthread_mutex_lock(&mouse_state_mutex);
			ret = ms.y_pos;
			pthread_mutex_unlock(&mouse_state_mutex);

			return ret;
		}
		unsigned int Mouse::get_mouse_state(unsigned short button_code) {
			register unsigned int ret = 0;

			pthread_mutex_lock(&mouse_state_mutex);
			ret = ms.button_state[button_code - MOUSE_EV_OFFSET];
			pthread_mutex_unlock(&mouse_state_mutex);

			return ret;
		}

		Mouse::Mouse() {
			fd = open_mse(get_default_mouse_device_file());

			// Initialize thread objects
			pthread_mutex_init(&mouse_state_mutex, NULL);
			pthread_attr_init(&attr);
			pthread_attr_setschedpolicy(&attr, SCHED_IDLE);

			// Launch thread
			if (pthread_create(&tid, &attr, (pthread_func_t)mouse_state_thread, this) < 0)
				ERRLQ(logger, "Failed to launch mouse state thread!");
		}

		Mouse::~Mouse() {
			pthread_kill(tid, SIGSEGV);

			pthread_mutex_destroy(&mouse_state_mutex);
			pthread_attr_destroy(&attr);

			pthread_join(tid, NULL);

			if (fd > 0) close(fd);

			DEBUG(logger, "Mouse class destroyed...");
		}
#else
		LONG Mouse::get_mouse_x() {
			POINT pointInfo;

			if (GetCursorPos(&pointInfo) == FALSE)
				ERRLQ(logger, "Failed to get cursor x position!");

			return pointInfo.x;
		}

		LONG Mouse::get_mouse_y() {
			POINT pointInfo;

			if (GetCursorPos(&pointInfo) == FALSE)
				ERRLQ(logger, "Failed to get cursor y position!");

			return pointInfo.y;
		}
#endif
	}
}