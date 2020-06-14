/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/--- Application.h --/-----*/
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

#pragma once

namespace Ventile {
	namespace System {
		// MALLOCS
		// -------------------
		VENTILEAPI void* ec_malloc(const unsigned int nBytes);

		// I/O
		// -------------------
		VENTILEAPI unsigned int get_file_size(const int fd, const long offset = 0);
		VENTILEAPI int open_file(const char* const file_name, const int flags, char** buf = NULL, const bool isproc = false);

#ifdef _WIN32
		VENTILEAPI bool is_key_pressed(const bool all = true, const int vk = 0);
#else
		class VENTILEAPI Keyboard {
		private:
			pthread_mutex_t key_state_mutex = { 0 };
			unsigned int key_state[255] = { 0 };
			kbd_thread_info kinfo = { 0 };

			char* get_default_keyboard_device_file();
			int open_kbd(const char* const kbd_dev);
			void launch_key_state_thread();
			void destroy_key_state_thread();
			void print_input_event(struct input_event* ie);
			static void sig_call(int sig);
			static void* key_state_thread(void* data);
			char is_key_pressed(const char any, const unsigned short code, const int kbd_fd);
		public:
			int get_key_state(unsigned short key_code);

			Keyboard();
			~Keyboard();
	};

#endif
	}

	class VENTILEAPI Application {
	public:
		Application();
		~Application();
		void run();
	};
}