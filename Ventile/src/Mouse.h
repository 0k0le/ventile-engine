/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/----- Mouse.h ------/-----*/
/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
  /*------------------------/-----------------------------------/-----------------*/
 /*-- Matthew Todd Geiger -/- matthewgeigersoftware@gmail.com -/- 425-363-0746 --*/
/*------------------------/-----------------------------------/-----------------*/

#pragma once

namespace Ventile{
	namespace System {
#ifndef _WIN32
        class VENTILEAPI Mouse {
        private:
            pthread_mutex_t mouse_state_mutex;
            pthread_attr_t attr;
            pthread_t tid;
            mouse_state ms;
            int fd;

            char* get_default_mouse_device_file();
            int open_mse(const char* const mse_dev);
            void print_input_event(struct input_event* ie);

            static void get_mouse_event(Mouse* mouse);
            static void* mouse_state_thread(void* data);
            static void mouse_thread_signal_handler(int sig);

        public:
            Mouse();
            ~Mouse();

            unsigned int get_mouse_x();
            unsigned int get_mouse_y();

            unsigned int get_mouse_state(unsigned short button_code);

        };
#else
        VENTILEAPI LONG get_mouse_x();
        VENTILEAPI LONG get_mouse_y();
#endif
	}
}