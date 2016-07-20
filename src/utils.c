/*   utils.c -   helper functions for skod.
*		    
* Copyright (c) 2016, 2015 by Hypsurus <hypsurus@mail.ru>     
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

#include "utils.h"

/* This function will print to stdout for multiple tasks.*/
void print(int type, int quit, char *format, ...) {
	char msg[MAX_STR];
	va_list li;

	va_start(li, format);
	vsprintf(msg, format, li);
	va_end(li);

	switch(type) {
		case PINFO:
			printf("%s%s%s %s\n", CYN, UNICODE_CHECK, END, msg);
			break;
		case PERROR:
			printf("%s%s%s %s\n", RED, UNICODE_X, END, msg);
			break;
		/* debug */
		case PDEBUG:
			printf("%s/d/%s %s", YEL, END, msg);
			break;
		case PSUCCESS:
			printf("%s%s%s %s", GREEN, UNICODE_CHECK, END, msg);
			break;
	}

	if ( quit ) exit(0);
}

/* Will check buffer size */
void chkbuffer(char *buffer) {
	int i = 0;
	
	if ( buffer ) {
		for ( i = 0; buffer[i] != 0; i++) {
			if ( i > MAX_STR-6 )
				print(PERROR, TRUE,"You cannot pass this string.");
		}
	}
}

int util_file_exists(char *filename) {
	struct stat buffer;
	return (stat(filename, &buffer) == 0);
}

void signal_handler(int sig) {
	if ( sig == SIGALRM ) {
		close(fd);
		fd = -1;
	}
	if ( sig == SIGINT ) {
		close(fd);
		print(PERROR, TRUE, "%ssInterrrupt ... %s\n", YEL,END);
	}
}

/* Makedirs */
void util_makedirs(char *path) {
	char **ptr = NULL;
	char *dirs[10];
	int i = 0;

	//dirs = &strchr(path, '/');
	ptr = dirs;

	for (i = 0; ptr[i] != 0; i++) {
		printf("%s\n", *ptr);
		ptr++;
	}

}

/* Calc bytes size for recived bytes.. */
void calc_bytes(data_t *data, float bytes) {                   
	if ( bytes >= 1024.0 ) {
		data->bytes = bytes / 1024;
		data->bytes_postfix = 0x08;
	}
	if ( bytes >= 1048576.0 ) {
		data->bytes = bytes / 1048576.0;
		data->bytes_postfix = 'M';
	}
	if ( bytes >= 1073741824.0 ) {
		data->bytes = bytes / 1073741824.0;
		data->bytes_postfix = 'G';
	}
}
