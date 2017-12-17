#ifndef unix
#define WIN32
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif
#include <stdio.h>
#include <string.h>
#define PROTOPORT 5193 
#define QLEN 6 
int visits = 0; 

main(argc, argv)
int argc;

char *argv[];
{
struct hostent *ptrh; 
struct protoent *ptrp; 
struct sockaddr_in sad; 
struct sockaddr_in cad; 
int sd, sd2; 
int port;
int alen; 
char buf[1000]; 
char ubuf[100]; 
#ifdef WIN32
WSADATA wsaData;
WSAStartup(0x0101, &wsaData);
#endif
memset((char *)&sad,0,sizeof(sad)); 
sad.sin_family = AF_INET; 
sad.sin_addr.s_addr = INADDR_ANY; 

if (argc > 1) { 
port = atoi(argv[1]); 
} else {
port = PROTOPORT; 
}
if (port > 0) 
sad.sin_port = htons((u_short)port);
else { 
fprintf(stderr,"bad port number %s\n",argv[1]);
exit(1);
}

if ( ((int)(ptrp = getprotobyname("tcp"))) == 0) {
fprintf(stderr, "cannot map \"tcp\" to protocol number");
exit(1);
}

sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
if (sd < 0) {
fprintf(stderr, "socket creation failed\n");
exit(1);
}
/* Bind a local address to the socket */
if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
fprintf(stderr,"bind failed\n");
exit(1);
}
/* Specify size of request queue */
if (listen(sd, QLEN) < 0) {
fprintf(stderr,"listen failed\n");
exit(1);
}

while (1) {
alen = sizeof(cad);
if ( (sd2=accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
fprintf(stderr, "accept failed\n");
exit(1);
}
if (recv(sd2, ubuf, sizeof(ubuf), 0) > 0) {
visits++;
sprintf(buf, "Hello %s, you are visitor number %d to this server\n",
ubuf, visits);
send(sd2,buf,strlen(buf),0);
}
closesocket(sd2);
