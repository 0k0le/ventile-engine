/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/--- EntryPoint.h ---/-----*/
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

#pragma once

#ifndef _ENGINE_DLL

void* operator new(size_t size) {
	void* ptr = Ventile::System::ec_malloc(size);
	if (ptr == NULL)
		ERRQ("Failed to allocate memory!");
	return ptr;
}

void operator delete(void* ptr) {
	free(ptr);
	ptr = nullptr; // Done for nullptr recognition for early free
}

// User must require signal handler and application spawner
extern Ventile::Application* CreateApplication();
extern void SignalHandler(int signal);
extern int vMain();

namespace Ventile {
	VENTILEAPI System::Logger* logger;
}

#ifdef _WIN32

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPSTR     lpCmdLine,
					int       nShowCmd ) {
	UNUSED(hInstance); UNUSED(hPrevInstance); UNUSED(lpCmdLine); UNUSED(nShowCmd);

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	// Setup signal handling
	if (signal(SIGINT, SignalHandler) == SIG_ERR)
		ERRQ("Failed to initialize signal handler");

	// Spawn application
	Ventile::Application* app = CreateApplication();

	if (vMain() == -1)
		ERRLQ(Ventile::logger, "V main failed!");

	int ret = app->run();
	delete app;

	FreeConsole();

	EXIT(ret);
}

#else

int main(const int argc, const char** const argv, const char** const envp) {
	UNUSED(argc); UNUSED(argv); UNUSED(envp);

	// Setup signal handling
	if (signal(SIGINT, SignalHandler) == SIG_ERR)
		ERRQ("Failed to initialize signal handler");

	// Spawn application
	Ventile::Application* app = CreateApplication();

	if (vMain() == -1)
		ERRLQ(Ventile::logger, "V main failed!");

	int ret = app->run();
	delete app;

	EXIT(ret);
}

#endif

#endif