/* utils.c - utils */

#include "utils.h"

void print(int type, int quit, char *format, ...) {
	char msg[MAX_STR];
	va_list li;

	va_start(li, format);
	vsprintf(msg, format, li);
	va_end(li);

	switch(type) {
		case PINFO:
			printf("%s%s%s\n", WHT, END, msg);
			break;
		case PERROR:
			printf("%s/X/%s %s\n", RED, END, msg);
			break;
		/* debug */
		case PDEBUG:
			printf("%s/d/%s %s", YEL, END, msg);
			break;
		case PSUCCESS:
			printf("%s/s/%s %s", GREEN, END, msg);
			break;
		/* print without new line */
		case PNNL:
			printf("%s", msg);
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
