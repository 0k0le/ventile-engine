  /*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
 /*--/ Vulkan API Test Software /--*/   /**/   /*-----/----- main.cpp -----/-----*/
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

// Ventile API - Ventile.h must be included first
#include "../../Ventile/src/Ventile.h"
#include "VentileApplication.h"

// User must create main class, signal handler, and application spawner

class Sandbox : public Ventile::Application {
public:
	Sandbox() {
		printf("Ventile Example Application\n");
		// Register engine kill key
#ifdef _WIN32
		Ventile::kill_key = VK_ESCAPE;
#else
		Ventile::kill_key = KEY_ESC;
#endif
	}

	~Sandbox() {
		printf("Ventile Example Application shutting down\n");
		fflush(stdout);
	}
};

// Register engine signal handler
void SignalHandler(int signal) {
	if (signal == SIGINT)
		Ventile::engine_running = false;

	return;
}

// Register engine application launcher
Ventile::Application* CreateApplication() {
	return new Sandbox();
}