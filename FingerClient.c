#ifndef unix
#define WIN32
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#define PROTOPORT 5193 
extern int errno;
char localhost[] = "192.168.98.30";
main(argc, argv)
int argc;
char *argv[];
{
struct hostent *ptrh; 
struct protoent *ptrp; 
struct sockaddr_in sad; 
int sd;
int port; 
char *host; 
int n; 
char buf[1000]; 
#ifdef WIN32
WSADATA wsaData;
WSAStartup(0x0101, &wsaData);
#endif
struct passwd *ppwd;
memset((char *)&sad,0,sizeof(sad)); 
sad.sin_family = AF_INET; 

if (argc > 2) { 
port = atoi(argv[2]); 
} else {
port = PROTOPORT; 
}
if (port > 0) 
sad.sin_port = htons((u_short)port);
else { 
fprintf(stderr,"bad port number %s\n",argv[2]);
exit(1);
}

if (argc > 1) {
host = argv[1]; 
} else {
host = localhost;
}

ptrh = gethostbyname(host);

if ( ((char *)ptrh) == NULL ) {
fprintf(stderr,"invalid host: %s\n", host);
exit(1);
}
memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) {
fprintf(stderr, "cannot map \"tcp\" to protocol number");
exit(1);
}

sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
if (sd < 0) {
fprintf(stderr, "socket creation failed\n");
exit(1);
}

if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
fprintf(stderr,"connect failed\n");
exit(1);
}

if ((ppwd = getpwuid(getuid())) == NULL) {
fprintf(stderr,"could not get user name\n");
exit(1);
}
/* send our user name */
send(sd, ppwd->pw_name, strlen(ppwd->pw_name)+1, 0);
n = recv(sd, buf, sizeof(buf), 0);
while (n > 0) {
write(1,buf,n);
n = recv(sd, buf, sizeof(buf), 0);
}

closesocket(sd);
exit(0);
}
