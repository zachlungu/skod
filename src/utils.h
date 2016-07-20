/*   utils.h - skod helper functions.
*		    
* Copyright (c) 2015, 2016 by Hypsurus <hypsurus@mail.ru>     
*
* skod is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 3 of the License, or
* (at your option) any later version.
*
* skod is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/


#ifndef UTILS_H
#define UTILS_H

#include "skod.h"

/* Give some colors to the output */
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GREEN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLUE  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
#define END "\033[0m"

/* Unicode */
#define UNICODE_LINE	"\xe2\x94\x80"
#define UNICODE_CHECK	"\xe2\x9c\x93"
#define UNICODE_X	    "x"


enum { PINFO ,PERROR, PDEBUG, PSUCCESS};

/* Handle data */
typedef struct {
	float bytes;
	int bytes_postfix;
} data_t;

/* prototypes */
int util_file_exists(char*);
void util_makedirs(char*);
void calc_bytes(data_t *, float);
void print(int mode,int exit, char*, ...);
void die(char*, ...);
void signal_handler(int);
void chkbuffer(char*);

#endif /*UTILS_H*/
