  /*--/--------------------------/--*/   /**/   /*-----/--------------------/-----*/
 /*--/ Vulkan API Test Software /--*/   /**/   /*-----/---- Ventile.cpp ---/-----*/
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
    namespace System {

        // MALLOCS
        // -------------------
        void* ec_malloc(const size_t nBytes) {
            // Allocate memory
            register void* ptr = NULL;
            if ((ptr = malloc(nBytes)) == NULL)
                return NULL;

            // Null out array
            for (unsigned int i = 0; i < nBytes; i++)
                *((char*)ptr + i) = 0;

            return ptr;
        }

        // I/O
        // -------------------
        unsigned int get_file_size(const int fd, const long offset) {
            lseek(fd, 0, SEEK_SET);
            const unsigned int flen = lseek(fd, 0, SEEK_END);
            lseek(fd, offset, SEEK_SET);

            return flen;
        }

        int open_file(const char* const file_name, const int flags, char** buf, const bool isproc) {
            int fd = 0;

            if ((fd = open(file_name, flags)) == -1)
                return -1;

            // Copy file to memory
            if (buf != NULL) {
                // Check if opening proc file
                if (isproc == false) {
                    register const unsigned int file_len = get_file_size(fd, 0) + 1;

                    // Allocate memory
                    if (!(*buf = (char*)ec_malloc(file_len)))
                        return -2;

                    // Read file contents
                    if (read(fd, *buf, file_len) == -1)
                        return -3;
                }
                else {
                    // Shitty solution but fuck it, not gonna waste an hour of my life on this shit
                    char temp_buf = 0;
                    register unsigned int byte_count = 0;
                    register int ret = 0;

                    // Get file length...
                    while ((ret = read(fd, &temp_buf, 1)) > 0)
                        byte_count++;

                    if (ret == -1)
                        return -3;

                    lseek(fd, 0, SEEK_SET);

                    if (!(*buf = (char*)ec_malloc(byte_count + 1)))
                        return -2;

                    if (read(fd, *buf, byte_count) == -1)
                        return -3;
                }
            }

            // Close file if read only
            if ((flags & 0x3) == O_RDONLY) {
                close(fd);
                fd = 0;
            }

            return fd;
        }

        // Percision Time
        // -------------------
        VENTILEAPI PERCISIONTIME get_percision_time() {
#ifdef _WIN32
            LARGE_INTEGER frequency, time;
            if (::QueryPerformanceFrequency(&frequency) == FALSE)
                return -1;
            if (::QueryPerformanceCounter(&time) == FALSE)
                return -2;

            return static_cast<PERCISIONTIME>(time.QuadPart) / frequency.QuadPart;
#else
            struct timespec ts;

            clock_gettime(CLOCK_MONOTONIC_RAW, &ts);

            return static_cast<PERCISIONTIME>((ts.tv_sec)
                + (ts.tv_nsec)
                / BILLION);
#endif
        }
    }
}