/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/--- Keyboard.cpp ---/-----*/
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
// ---

#include "Ventile.h"

namespace Ventile {
	namespace System {
#ifndef _WIN32

		void Keyboard::sig_call(int sig) {
			if (sig == SIGSEGV)
				pthread_exit(NULL);
		}

		char* Keyboard::get_default_keyboard_device_file() {
			static char device_file[DEVICE_FILE_DIR_LENGTH] = { 0 };
			DIR* dp = NULL;
			struct dirent* ep = NULL;

			memset(device_file, 0, DEVICE_FILE_DIR_LENGTH);

			if ((dp = opendir(INPUT_DEVICE_DIRECTORY)) == NULL)
				ERRQ("Failed to open directory: " INPUT_DEVICE_DIRECTORY "\n");

			while ((ep = readdir(dp))) {
				if (strstr(ep->d_name, "kbd") && strlen(ep->d_name) < DEVICE_FILE_DIR_LENGTH) {
					sprintf(device_file, INPUT_DEVICE_DIRECTORY "%s", ep->d_name);
					break;
				}
			}

			closedir(dp);

			return device_file;
		}

		int Keyboard::open_kbd(const char* const kbd_dev) {
			int fd = 0;

			// Read only
			if ((fd = open(kbd_dev, O_RDONLY | O_NOCTTY)) == -1)
				ERRQ("Failed to open keyboard device!");

			// Non blocking
			int flags = fcntl(fd, F_GETFL, 0);
			if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) < 0)
				ERRQ("Failed to enable non-blocking!");

			return fd;
		}

		void Keyboard::print_input_event(struct input_event* ie) {
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

		char Keyboard::is_key_pressed(const char any, const unsigned short code, const int kbd_fd) {
			char not_blocking = 1;
			struct input_event ie;

			// Read data from device until EV_KEY event is found or continue if no data is readable
			do {
				if (read(kbd_fd, &ie, sizeof(ie)) <= 0) {
					if (errno != EAGAIN)
						ERRQ("Failed to read from KBD Device!");

					not_blocking = 0;
				}

			} while (ie.type != EV_KEY && not_blocking);

			// Copy key data to array
			if (not_blocking) {
				pthread_mutex_lock(&key_state_mutex);
#ifdef _DEBUG
				print_input_event(&ie);
#endif
				key_state[ie.code] = ie.value;
				pthread_mutex_unlock(&key_state_mutex);
			}

			return ((ie.code == code || any) && not_blocking) ? 1 : 0;
		}

		void* Keyboard::key_state_thread(void* data) {
			Keyboard* keyboard = (Keyboard*)data;

			signal(SIGSEGV, keyboard->sig_call);

			while (1) keyboard->is_key_pressed(1, 0, keyboard->kinfo.kbd_fd);
		}

		int Keyboard::get_key_state(unsigned short key_code) {
			register int state = 0;

			pthread_mutex_lock(&key_state_mutex);
			state = key_state[key_code];
			pthread_mutex_unlock(&key_state_mutex);

			return state;
		}

		void Keyboard::launch_key_state_thread() {
			kinfo.kbd_fd = open_kbd(get_default_keyboard_device_file());

			// Initialize mutex for key state array
			pthread_mutex_init(&key_state_mutex, NULL);

			pthread_attr_init(&(kinfo.attr));

			// Set low priority
			pthread_attr_setschedpolicy(&(kinfo.attr), SCHED_IDLE);

			if (pthread_create(&(kinfo.tid), &(kinfo.attr), (pthread_func_t)(&key_state_thread), this) < 0)
				ERRQ("Failed to launch key state thread!");
		}

		void Keyboard::destroy_key_state_thread() {
			pthread_kill(kinfo.tid, SIGSEGV);

			pthread_mutex_destroy(&key_state_mutex);
			pthread_attr_destroy(&kinfo.attr);

			pthread_join(kinfo.tid, NULL);

			if (kinfo.kbd_fd > 0) close(kinfo.kbd_fd);

#ifdef _DEBUG
			printf("Keyboard device shutdown...\n");
#endif
		}

		Keyboard::Keyboard() {
			launch_key_state_thread();
		}

		Keyboard::~Keyboard() {
			destroy_key_state_thread();
		}

#else 

		bool is_key_pressed(const bool all, const int vk) {
			if (all) {
				for (int i = 0; i < 255; i++) {
					if (GetAsyncKeyState(i) & (1 << 15)) {
						return true;
					}
				}
			}
			else {
				if (GetAsyncKeyState(vk) & (1 << 15))
					return true;
			}

			return false;
		}

#endif

	}
}
