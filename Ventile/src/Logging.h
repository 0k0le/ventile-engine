/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
/*--/ Vulkan API Test Software /--*/   /**/   /*-----/----- Logging.h ----/-----*/
/*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
  /*------------------------/-----------------------------------/-----------------*/
 /*-- Matthew Todd Geiger -/- matthewgeigersoftware@gmail.com -/- 425-363-0746 --*/
/*------------------------/-----------------------------------/-----------------*/

#pragma once

namespace Ventile {
	namespace System {
		class VENTILEAPI Logger {
		private:
			char* log_file_name;
			FILE* fp;

		public:
			Logger(const char* const file_name = "log.txt");
			~Logger();

			void log(int log_code, const char* fmt, ...);
		};
	}
}
