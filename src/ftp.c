/*   ftp.c -   File transfer protocol functions (RFC 959)            
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

#include "ftp.h"

/* TODO: Upload files and folders. */
void ftp_upload(ftp_t *ftp, char *dest, char *path) {
	ftp_upload_single(ftp, dest, path);
}

/* Upload single file to the FTP server. */
void ftp_upload_single(ftp_t *ftp, char *dest, char *path) {
	upload_t u;

	ftp_close(ftp);
	ftp_mkcon(ftp);
	u.file_name = strrchr(path, '/')+1;

	if (( u.in_file = fopen(path, "r")) == NULL ) 
		print(PERROR, TRUE, "Failed to read: %s", u.file_name);
	
	fprintf(ftp->FD, "TYPE I\r\n");
	ftp_getline(ftp);
	ftp->dataport = ftp_getdataport(ftp);
	fprintf(ftp->FD, "STOR %s/%s\r\n", dest, u.file_name);
	u.data = tcp_connect(ip, ftp->dataport, "wb");

	while(( fgets(u.buffer, sizeof(u.buffer), u.in_file)) != NULL ) {
		u.up_size += fwrite(u.buffer, strlen(u.buffer), 1, u.data);
		printf("%s%s%s Uploading %s\'%s\'%s %d\r", BLUE, UNICODE_CHECK, END,
		YEL, u.file_name, END, u.up_size);
	}

	putchar(0x0a);
	fclose(u.in_file);
	fclose(u.data);
}

/* Download files and folders. */
void ftp_download(ftp_t *ftp, char *dest, char *path) {
	char buffer[MAX_STR];
	FILE *data = NULL;
	char *line = NULL;
	char *folder_name = NULL;

	ftp_close(ftp);
	ftp_mkcon(ftp);
	ftp->dataport = ftp_getdataport(ftp);

	fprintf(ftp->FD, "NLST %s\r\n", path);
	ftp_getline(ftp);
	data = tcp_connect(ip, ftp->dataport, "r");
	
	while (( fgets(buffer, sizeof(buffer), data)) != NULL ) {
		ftp_close(ftp);
		ftp_mkcon(ftp);
		strtok(buffer, "\r\n");

		fprintf(ftp->FD, "SIZE %s\r\n", buffer);
		line = ftp_getline(ftp);
		ftp->code = atoi(line);
		if ( ftp->code == 213 ) {
			ftp_download_single(ftp, dest, buffer);
		}
		else if ( ftp->code == 550 && strstr(line, "plain")) {
			folder_name = strrchr(buffer, '/')+1;	
			mkdir(folder_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
			chdir(folder_name); /* Change folder for downloading the file. */
			print(PINFO, FALSE, "Downloading folder: %s%s%s", GREEN, buffer, END);
			ftp_download(ftp, dest, buffer);
			chdir("..");
			chdir(dest);
		}
		else if ( ftp->code == 550 && strstr(line, "denied")) {		
			print(PERROR, FALSE, "%s - %sAccess denied%s", buffer, RED, END);
		}
	}
}

/* Download a singal file from remote host. */
void ftp_download_single(ftp_t *ftp, char *dest, char *path) {
	download_t d;
	char *line = NULL;

	ftp_close(ftp);
	ftp_mkcon(ftp);

	d.file_size = ftp_size(ftp, path);
	d.dl_size = 0;

	fprintf(ftp->FD, "TYPE I\r\n");
	ftp_getline(ftp);

	ftp->dataport = ftp_getdataport(ftp);
	d.data = tcp_connect(ip, ftp->dataport, "r");	
	fprintf(ftp->FD, "RETR %s\r\n", path);
	line = ftp_getline(ftp);
	ftp->code = atoi(line);

	if ( ftp->code == 150 ) {
		d.file_name = strrchr(path, '/')+1;	
		
		/* Check if file already exists in the target directory */
		if ( util_file_exists(d.file_name) ) {
			print(PERROR, FALSE, "%s - already exists in %s.", d.file_name, dest);
			return;
		}

		else if (( d.out_file = fopen(d.file_name, "wb")) == NULL ) {
			print(PERROR, FALSE, "Failed to create: %s.", d.file_name);
			return;
		}
	} 
	else {
		print(PERROR, FALSE, "Failed to download %s (%s).", d.file_name, line);
		return;
	}

	while (( fgets(d.buffer, sizeof(d.buffer), d.data)) != NULL ) {
		d.dl_size += fwrite(d.buffer, 1, strlen(d.buffer), d.out_file);
		printf("%s%s%s Downloading %s\'%s\'%s %d of %d\r", BLUE, UNICODE_CHECK, END, 
			YEL, d.file_name, END,
			d.dl_size, d.file_size);
	}

	fclose(d.out_file);
	fclose(d.data);
	putchar(0x0a);
}

/* Mdtm - return the modification time of a file*/
void ftp_mdtm(ftp_t *ftp, char *path) {
	char *line = NULL;
	char act[MAX_STR];

	ftp_close(ftp);
	ftp_mkcon(ftp);
	fprintf(ftp->FD, "MDTM %s\r\n", path);
	line = ftp_getline(ftp);
	ftp->code = atoi(line);
	if ( ftp->code == 550 )
		print(PERROR, TRUE,"Failed to run MTDM on \'%s\'.", path);
	sprintf(act, "%s", strrchr(line, 0x20)+1);
	print(PSUCCESS, FALSE, "%s %c%c:%c%c:%c%c %c%c/%c%c/%c%c%c%c\n",
			path,
			act[8], act[9], act[10], act[11], act[12], act[13],
			act[6], act[7],
			act[4], act[5],
			act[0], act[1],act[2], act[3]);
}

/*
* When the download starts skod will change to the --dest,-e folder.
*/
void ftp_cwd(ftp_t *ftp, char *path) {
	char *line = NULL;

	/* if download */ 
	if ( flag == SKOD_DOWNLOAD ) {
		if ( chdir(path) == -1 )
			print(PERROR, TRUE,"Cannot change direcotry to \'%s\' ...", path);
	}

	/* if upload */
	if ( flag == SKOD_UPLOAD ) {
		fprintf(ftp->FD, "CWD %s\r\n", path);
		line = ftp_getline(ftp);
		ftp->code = atoi(line);
		if ( ftp->code == 550 )
			print(PERROR, TRUE,"Failed to change directory to %s\'%s\'%s.", RED,END);
	}
}

void ftp_delete(ftp_t *ftp, char *path) {
	char *line = NULL;

	fprintf(ftp->FD, "DELE %s\r\n", path);
	line = ftp_getline(ftp);
	ftp->code = atoi(line);
	if ( ftp->code == 250 )
		print(PSUCCESS, FALSE, "%s\'%s\'%s deleted!", path);
	else if ( ftp->code ==  550 )
		print(PERROR, TRUE, "\'%s\' No such file or directory.", path);
}

/* Print name of the current directory on the remote host. */
char * ftp_pwd(ftp_t *ftp) {
	char *ppwd = NULL;
	char *line = NULL;
	static char pwd[MAX_STR];

	fprintf(ftp->FD, "PWD\r\n");
	line = ftp_getline(ftp);
	ppwd = strstr(line, "\"");
	sscanf(ppwd, "\"%s\" %[^\n]s", pwd, line);
	strtok(pwd, "\"");

	return pwd;
}

/* Get file size on the remote host */
int ftp_size(ftp_t *ftp, char *path) {
	char *line = NULL;
	int file_size = 0;
		
	fprintf(ftp->FD, "SIZE %s\r\n", path);
	line = ftp_getline(ftp);
	if ( atoi(line) != 213 )
		print(PERROR, TRUE, "Failed to get \'%s\' size.", path);

	sscanf(line, "213 %d", &file_size);
	return (file_size);
}

/* Display the contents of single file from the remote host */
void ftp_cat(ftp_t *ftp, char *path) {
	char *line = NULL;
	FILE *data;
	char buffer[MAX_STR];

	ftp->dataport = ftp_getdataport(ftp);
	fprintf(ftp->FD, "TYPE I\r\n");
	line = ftp_getline(ftp);
	if ( atoi(line) == 500 )
		print(PERROR, TRUE,"Failed to set TYPE.");
	fprintf(ftp->FD, "RETR %s\r\n", path);
	/* SIGALRM */
	ftp_getline(ftp);

	data = tcp_connect(ip, ftp->dataport, "r");
	while(( fgets(buffer, sizeof(buffer), data)) != NULL ) {
		printf("%s", buffer);
	}
	fclose(data);
	close(dfd);
}

/* 
 * Get list of files 
 * 
 * int opt:
 * 	1 - NLST - /bin/ls
 * 	2 - LIST /bin/ls -alh 
 *
 */
void ftp_list(ftp_t *ftp, char *path, int opt) {
	FILE *data;
	char buffer[MAX_STR];

	ftp->dataport = ftp_getdataport(ftp);
		
	switch(opt) {
		case 1:
			fprintf(ftp->FD, "LIST %s\r\n", path);
		case 2:
			fprintf(ftp->FD, "NLST %s\r\n", path);
	}
	/* SIGALRM */
	ftp_getline(ftp);

	data = tcp_connect(ip, ftp->dataport, "r");

	while ( fgets(buffer, sizeof(buffer), data) != NULL ) {
		strtok(buffer, "\n");

		/* dir*/
		if ( buffer[0] == 0x64 )
			printf("%s%s%s\n", BLUE,buffer, END);
		/* symlink*/
		if ( buffer[0] == 0x6c )
			printf("%s%s%s\n", CYN,buffer,END);
		/* File */
		else if ( buffer[0] == 0x2d )
			printf("%s%s%s\n", WHT,buffer, END);
	}
	fclose(data);
	close(dfd);
	/* No need to close fd the SIGALRM do it */
}

/* 
* When running PASV the server will return data port. 
*/
char * ftp_getdataport(ftp_t *ftp) {
	char *line = NULL;
	int p1 = 0;
	int p2 = 0;
	char *act = NULL;
	static char port[MAX_STR];

	fprintf(ftp->FD, "PASV\r\n");
	line = ftp_getline(ftp);

	act = strrchr(line, ',') -5;
	/* p3_secure is for secureing the data port,
	 * in case it will be 3 digits */
	sscanf(act, "%*d,%d,%d)", &p1, &p2);
	
	#ifdef DEBUG
		print(PDEBUG, FALSE, "Dataport: %d\n", (p1*256+p2));
	#endif
	sprintf(port, "%d", (p1*256+p2));
	return port;
}

/* Will try to login */
int ftp_login(ftp_t *ftp) {
	char *line = NULL;

	fprintf(ftp->FD, "USER %s\r\n", ftp->user);
	line = ftp_getline(ftp);
	ftp->code = atoi(line);

	/* case it will login anonymous user without PASS */
	if ( ftp->code == 220 || ftp->code == 230 ) {
		ftp->logged = 1;
		#ifdef DEBUG
			print(PDEBUG, FALSE, "Login success!\n");
		#endif
		return ftp->code;
	}
	fprintf(ftp->FD, "PASS %s\r\n", ftp->password);
	line = ftp_getline(ftp);
	ftp->code = atoi(line);

	if ( ftp->code == 220 || ftp->code == 230 ) {
		ftp->logged = 1;
		#ifdef DEBUG
			print(PDEBUG, FALSE, "Login success!\n");
		#endif
	}
	else 
		ftp->logged = 0;
	return ftp->code;
}

/* Will grab the banner */
int ftp_banner(ftp_t *ftp) {
	char *line = NULL;
	line = ftp_getline(ftp);
	ftp->code = atoi(line);
	return (ftp->code = 0);
}

/* Will return line from the socket */
char * ftp_getline(ftp_t *ftp) {
	static char line[MAX_STR];
	char *s = NULL;

	while (( fgets(line, sizeof(line), ftp->FD)) != NULL ) {
    	/* Avoid the comments when connecting to server */
		if (strtol(line, &s, 10) != 0 && s != NULL) {
			if (isspace(*s)) {
				#ifdef DEBUG
					print(PDEBUG, FALSE, "%s", line);
				#endif
            	return line;
            }
        }
	}		
	return "None\n";
}

/* handle TCP connection */
void ftp_mkcon(ftp_t *ftp) {
	/* Make the connection */
	ftp->FD = tcp_connect(ftp->server, ftp->port, "r+");
	if ( !ftp->FD) 
		print(PERROR, TRUE,"Connection failed.");
	ftp_banner(ftp);
	ftp_login(ftp);

	/* Check if logged IN and not scanning */
	if ( ftp->logged != 1 && flag != 99 ) {
		print(PERROR, TRUE,"%sNo logged in%s, try to reconnect.", RED,END);
	}
}

/* close connection */
void ftp_close(ftp_t *ftp) {
	fprintf(ftp->FD, "QUIT\r\n");
	#ifdef DEBUG
		print(PDEBUG, FALSE, "%s", ftp_getline(ftp));
	#endif
	fclose(ftp->FD);
	ftp->logged = 0;
}


