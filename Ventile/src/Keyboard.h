/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/---- Keyboard.h ----/-----*/
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

#pragma once

namespace Ventile {
	namespace System {

#ifdef _WIN32
		class VENTILEAPI Keyboard {
		public:
			bool get_key_state(const int vk = 0);
		};
#else
		class VENTILEAPI Keyboard {
		private:
			pthread_mutex_t key_state_mutex;
			unsigned int key_state[255];
			kbd_thread_info kinfo;

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
}
