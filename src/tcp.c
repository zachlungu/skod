/* tcp.c - TCP/IP fucntion create the connection to the FTP server. */

#include "tcp.h"

/* Connect to TCP/IP socket */
FILE * tcp_connect(char *server, char *port, char *mode) {
	struct addrinfo ai, *srv = NULL, *p = NULL;
	struct timeval timeout;

	/* Set time-out*/
	timeout.tv_sec = 2;
	timeout.tv_usec = 0;

	memset(&ai, 0, sizeof(struct addrinfo));
	if (( getaddrinfo(server, port, &ai, &srv)) != 0 ) {
		print(PERROR, TRUE,"Cannot resolve %s - (%s)", server,gai_strerror(1));
	}

	ai.ai_family = AF_UNSPEC;
	ai.ai_socktype = SOCK_STREAM;
	ai.ai_protocol = IPPROTO_TCP;

	p = srv;
	if (( getnameinfo((struct sockaddr *)p->ai_addr,
					p->ai_addrlen, 
					ip, sizeof(ip),
					NULL, (socklen_t) 0U, 
					NI_NUMERICHOST)) != 0)
		print(PERROR, TRUE,"Cannot resolve %s- (%s).", server,gai_strerror(1));

	/* skod.h //int fd */
	if (( fd = socket(p->ai_family, p->ai_socktype, 0)) < 0 )
		print(PERROR, TRUE,"Failed to create socket.");

	if ( connect(fd, p->ai_addr, p->ai_addrlen) < 0 )
		print(PERROR, TRUE,"Failed to connect.");
	freeaddrinfo(srv);

	if (setsockopt (fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout,
				sizeof(timeout)) < 0)
		print(PERROR, TRUE,"Socket timeout.");
	if (setsockopt (fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&timeout,
				sizeof(timeout)) < 0)
		print(PERROR, TRUE,"Socket timeout.");

	/* call to fdopen to return FILE */
	return (fdopen(fd, mode));
}

