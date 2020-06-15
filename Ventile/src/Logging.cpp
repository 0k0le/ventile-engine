#include "Ventile.h"

namespace Ventile {
	namespace System {
		void Logger::log(int log_code, const char* fmt, ...) {
			va_list arg, arg2;
			va_start(arg, fmt);
			va_copy(arg2, arg);

			if (log_code == LOGFAIL) {
				fprintf(stderr, "LOG -- ERROR --> ");
				vfprintf(stderr, fmt, arg);
				fprintf(stderr, "\n");
			}
			else {
				fprintf(stdout, "LOG -- GENERAL --> ");
				vfprintf(stdout, fmt, arg);
				fprintf(stdout, "\n");
			}

			va_end(arg);

			fprintf(fp, "LOG -- GENERAL --> ");
			vfprintf(fp, fmt, arg2);
			fprintf(fp, "\n");

			va_end(arg2);
		}

		Logger::Logger(const char* const file_name) {
			log_file_name = (char*)ec_malloc(strlen(file_name) + 2);
			strcpy(log_file_name, file_name);

			remove(log_file_name);

			fp = fopen(log_file_name, "w+");
			if (fp == NULL)
				ERRQ("Failed to open file %s", log_file_name);

			return;
		}

		Logger::~Logger() {
			free(log_file_name);
			fclose(fp);
		}
	}
}