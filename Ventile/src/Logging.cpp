#include "Ventile.h"

namespace Ventile {
	namespace System {
		void Logger::log(int log_code, const char* const fmt, ...) {
			va_list args;
			va_start(args, fmt);

			if (log_code == LOGFAIL) {
				fprintf(stderr, "LOG -- ERROR --> ");
				vfprintf(stderr, fmt, args);
				fprintf(stderr, "\n");
			}
			else {
				fprintf(stdout, "LOG -- GENERAL --> ");
				vfprintf(stdout, fmt, args);
				fprintf(stdout, "\n");
			}

			fprintf(fp, "LOG -- GENERAL --> ");
			vfprintf(fp, fmt, args);
			fprintf(fp, "\n");

			va_end(args);
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