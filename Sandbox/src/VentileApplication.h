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

#include "../../Ventile/src/Ventile.h"
#undef VENTILEAPI

static inline bool VentileApplicationMainLoop(Ventile::Application* app);

#ifdef  _WIN32
#define VENTILEAPI __declspec(dllimport)
#else
#define VENTILEAPI extern
#endif

namespace Ventile {
	VENTILEAPI bool engine_running;

#ifdef _WIN32
	VENTILEAPI int kill_key;
#else
	VENTILEAPI unsigned short kill_key;
#endif
}

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

	// GAME LOOP
	void app_proc() { 
		if (!VentileApplicationMainLoop(this))
			Ventile::engine_running = false;
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