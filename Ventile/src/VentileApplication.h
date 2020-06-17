/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*----- VentileApplication.h -----*/
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

// INCLUDE THIS HEADER AFTER Ventile.h

#pragma once

#ifdef _IS_ENTRY

#include "Ventile.h"
#undef VENTILEAPI

FORCEINLINE int vMainLoop(Ventile::Application* app, double deltaTime);

#ifdef  _WIN32
#define VENTILEAPI __declspec(dllimport)
#else
#define VENTILEAPI extern
#endif

namespace Ventile {
	VENTILEAPI bool engine_running;
	VENTILEAPI KILLKEYTYPE kill_key;
	// logger in entrypoint.h
}

class Sandbox : public Ventile::Application {
public:
	Sandbox() {
		DEBUG(Ventile::logger, "Launching Sandbox application startup procedure");

		// Register engine kill key
#ifdef _WIN32
		Ventile::kill_key = VK_F1;
#else
		Ventile::kill_key = KEY_F1;
#endif
	}

	void deconstruct() {
		DEBUG(Ventile::logger, "Launching Sandbox application shutdown procedure");
	}

	// GAME LOOP
	int app_proc(double deltaTime) {
		int ret = 0;

		if ((ret = vMainLoop(this, deltaTime)) != EXIT_LOOP || Ventile::engine_running == false) {
			Ventile::engine_running = false;
			deconstruct();
		}

		return ret;
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

#endif