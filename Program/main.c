#include <stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<signal.h>
#include<fcntl.h>
#include "Common.h"
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

//Constants
#define BYTES 1024

//Variables
int listenfd;
int _port; //Port loaded from file
char* _logPath;//LOg path loaded from file
char mesg[99999], *reqline[3], data_to_send[BYTES], path[99999];
int rcvd, fd, bytes_read;
char port[6];
char root [PATH_MAX + 1] = ""; //File system root

/*
 * Method for: Starts server, makes the socket, bind and listen.
 * Param port: specify port to be used
 */
void startServer(char *port)
{
    struct addrinfo hints, *res, *p;

    // getting addrinfo for host
    memset (&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo( NULL, port, &hints, &res) != 0)
    {
        writeFile("Error in method getaddrinfo \n",_logPath);
        exit(1);
    }

    //Method Socket and bind
    for (p = res; p!=NULL; p=p->ai_next)
    {
        int option = 1;
        listenfd = socket (p->ai_family, p->ai_socktype, 0);
        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));
        if (listenfd == -1) continue;
        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0) break;
    }
    if (p == NULL)
    {
        writeFile("Error in method socket or bind \n", _logPath);
	perror ("socket() or bind()");
        exit(1);
    }

    freeaddrinfo(res);

    // listen for connections
    if ( listen (listenfd, 1000000) != 0 )
    {
        writeFile("Error in method listen \n",_logPath);
        exit(1);
    }
}

/*
 * Method for: Send response to client
 * Param request: indicator for client
 */
void SendResponse(int request)
{
    memset( (void*)mesg, (int)'\0', 99999 );
    rcvd=recv(request, mesg, 99999, 0);

    //Receive error
    if (rcvd<0){
        writeFile("Error in recv method \n",_logPath);
    }
    //Message received
    else if (rcvd!=0) {
        writeFile(mesg, _logPath);//Write message
        reqline[0] = strtok (mesg, " \t\n");
        if ( strncmp(reqline[0], "GET\0", 4)==0 )
        {
            reqline[1] = strtok (NULL, " \t");
            reqline[2] = strtok (NULL, " \t\n");
            if ( strncmp( reqline[2], "HTTP/1.0", 8)!=0 && strncmp( reqline[2], "HTTP/1.1", 8)!=0 )
            {
                write(request, "HTTP/1.0 400 Bad Request\n", 25);
            }
            else
            {
                //if no file, open index
                if ( strncmp(reqline[1], "/\0", 2)==0 ){
                    reqline[1] = "/index.html";
                }

                strcpy(path, root);
                strcpy(&path[strlen(root)], reqline[1]);

                //Write message
                char message[100] = "";
                strcat(message, "File loaded: ");
                strcat(message, path);
                strcat(message, "\n");
                writeFile(message,_logPath);

                //File found
                if ( (fd=open(path, O_RDONLY))!=-1 )
                {
                    send(request, "HTTP/1.0 200 OK\n\n", 17, 0);
                    while ( (bytes_read=read(fd, data_to_send, BYTES))>0 )
                        write (request, data_to_send, bytes_read);
                }

                //File not found
                else{
                    write(request, "HTTP/1.0 404 Not Found\n", 23);
                }
            }
        }
    }

    //Close socket
    //shutdown (request, SHUT_RDWR);
    close(request);
}

/*
 * Main method, start server, accept conections and respond
 */
int main() {

    char preRoot [PATH_MAX + 1] = "";//Stores root with server name
    int lenPreRoot = 0;//Stores full path length
    int lenServerName = 9;//Stores server name length

    //Load variables
    _port = getPort();
    _logPath = getLogPath();
    sprintf(port, "%d", _port);//Get port in char
    if (readlink("/proc/self/exe", preRoot, sizeof(preRoot) - 1) == -1){//Get file system root
        strcpy(preRoot,"/home/server");//If error set default
    }

    lenPreRoot = strlen(preRoot);
    strncpy( root, &preRoot[0], lenPreRoot-lenServerName);//Remove server name from path
    //Start server
    struct sockaddr_in clientaddr;
    socklen_t addrlen;
    startServer(port);

    //Message for server started
    char message[60] = "";
    strcat(message, "Web server iniciado en el puerto: ");
    strcat(message, port);
    strcat(message, ", en el directorio: ");
    strcat(message, root);
    strcat(message, "\n");
    writeFile(message, _logPath);

    //Wait for connections
    while (1)
    {
        addrlen = sizeof(clientaddr);
        int value = accept (listenfd, (struct sockaddr *) &clientaddr, &addrlen);
        if (value<0)
            writeFile("Error on accept method",_logPath);
        else
            SendResponse(value);
    }

    return 0;
}
