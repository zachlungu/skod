/*   skod_data.c - Data for skod FTP scanner.
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

/* OS */
#define SKOD_OS_NIX	"Linux" /* *NIX 8*/
#define SKOD_OS_NT	"Windows NT"
#define SKOD_OS_UNKNOWN "unknown"

/* for The fingerprints */
typedef struct {
	char *prod;
	char *print;
} hcf_t;

const char *ftp_commands[] = {
	"NOTEXISTS", /* This is just to see how to server will responed to unvaild command. */
	"PASS",
	"ALLO", 
	"USER", /* This changes from server to server. */
};

const hcf_t hcf_fingerprints[] = {
	{"inetutils (FTPD)", "3295:1948:1933:1956"},
	{"vsFTPD",           "1769:1861:2088:2859"}, /* Logged*/
	{"vsFTPD",           "1769:1769:2088:1769"}, 
	{"vsFTPD 3.0.2+",    "2933:2169:2933:2909"}, 
	{"ProFTPD",          "2421:2123:2887:3382"},
	{"ProFTPD",          "2421:2543:3280:2543"},
	{"PureFTPD",         "2038:2607:673:2665"},
	{"PureFTPD",         "2038:2607:858:2665"},
	{"PureFTPD",         "1723:2370:2006:2557"}, /* Logged */
	{"Microsoft FTPD",   "3402:2169:2436:3483"},
};

const char *ftp_prod[] = {
	"ProFTPD",
	"vsFTPD",
	"PureFTPD",
};
