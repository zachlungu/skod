/*   ftp.h -   File transfer protocol headers.           
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


#ifndef FTP_H
#define FTP_H

#include "skod.h"

#define FTP_PORT "21"
#define FTP_USER	"anonymous"
#define FTP_PASSWORD	"anonymous"
#define FTP_SERVER	"localhost"

typedef struct {
	FILE *FD;
	int code;
	char *dataport;
	unsigned int alarm_sec;
	int logged;
	char *user;
	char *password;
	char *server;
	char *port;
} ftp_t;

typedef struct {
	FILE *out_file;
	FILE *data;
	char buffer[MAX_STR];
	char *file_name;
	int dl_size;
	int file_size;
} download_t;

typedef struct {
	FILE *in_file;
	FILE *data;
	int up_size;
	int file_size;
	char *file_name;
	char buffer[MAX_STR];
} upload_t;

/* prototypes */
char * ftp_getline(ftp_t*);
char * ftp_getdataport(ftp_t*);
char * ftp_pwd(ftp_t *ftp);
int ftp_banner(ftp_t*);
int ftp_login(ftp_t*);
int ftp_size(ftp_t*, char*);
void ftp_list(ftp_t*, char*,int);
void ftp_remove(ftp_t*, char*);
void ftp_mkcon(ftp_t *);
void ftp_close(ftp_t*);
void ftp_cat(ftp_t*, char*);
void ftp_download_single(ftp_t*, char*, char*);
void ftp_download(ftp_t*, char*, char*);
void ftp_upload(ftp_t*, char*, char*);
void ftp_upload_single(ftp_t*, char*, char*);
void ftp_delete(ftp_t*, char*);
void ftp_cwd(ftp_t*, char*);
void ftp_mdtm(ftp_t*, char*);

#endif /*FTP_H*/
