/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/-- Application.cpp -/-----*/
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
	VENTILEAPI bool engine_running = true;
    VENTILEAPI KILLKEYTYPE kill_key = 0;

	Application::Application() {

        keyboard = new System::Keyboard();
        mouse = new System::Mouse();
        logger = new System::Logger();

        logger->log(LOGSUCCESS, "Ventile Engine Version: " ENGINE_VERSION "\n");
	}

	Application::~Application() {
		logger->log(LOGSUCCESS, "Engine shutting down\n");

        delete keyboard;
        delete mouse;
        delete logger;

        SLEEP(1000);
	}

	void Application::run() {

        while (engine_running) {
            if (kill_key && keyboard->get_key_state(kill_key))
                engine_running = false;

            app_proc();
        }
	}
}