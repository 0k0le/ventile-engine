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

// Ventile Engine API
#include "VentileApplication.h"

#ifdef _WIN32
#define KILLCODE1 VK_SHIFT
#define KILLCODE2 VK_CONTROL
#else
#define KILLCODE1 KEY_LEFTSHIFT
#define KILLCODE2 KEY_LEFTCTRL
#endif

// Application main loop
static inline bool VentileApplicationMainLoop(register Ventile::Application* app) {
	// Disable mouse coords
	printf("X --> " KILLKEYPRINTF " | Y --> " KILLKEYPRINTF "\n",
		app->mouse->get_mouse_x(), app->mouse->get_mouse_y());

	// Kill engine if CTRL and SHIFT are pressed
	if (app->keyboard->get_key_state(KILLCODE1) && app->keyboard->get_key_state(KILLCODE2))
		return false;

	return true;
}
