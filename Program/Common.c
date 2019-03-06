/*
 * File: Common.c
 * Authors: Raul Arias Quesada - Bryan Abarca Weber - Rony Panigua Chacon
 * Description: Common methods implementation
 */

//Includes
#include <stdio.h>
#include <time.h>
#include <string.h>

//Constants
#define CONFPATH "/etc/webserver/config.conf"

//Variables
FILE *fp;
char date[64];

/*
 * Method for: Get port from config file
 * Return: If doesnt find the port set 8081
 */
int getPort(){

    static int port = 8081;//Static for just one value
    FILE *file = fopen (CONFPATH, "r");//Read file

    if (file != NULL)
    {
        char line[256];
        while(fgets(line, 256, file) != NULL)//Read line
        {
            if(line[0] == '#') continue;//If comment ignore
            sscanf(line, "PORT = %d", &port);//If line is port read it
        }
    }

    return port;
}

/*
 * Method for: Get path from config file
 * Return: If doesnt find the path set /var/log/syslog
 */
char* getLogPath(){

    static char path[100] = "/var/log/syslog";//Static for just one value
    FILE *file = fopen (CONFPATH, "r");//Read file

    if (file != NULL)
    {
        char line[256];
        while(fgets(line, 256, file) != NULL)//Read line
        {
            if(line[0] == '#') continue;//If comment ignore
            sscanf(line, "LOGFILE = %s", path);//If line is path read it
        }
    }

    return path;
}

/*
 * Method for: write to a file
 * Params message: Message to write
 *        file: Full file path
 */
void writeFile(char* message, char * file){
    fp = fopen(file, "a+");

    if(fp != NULL){

        //Get time
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(date, sizeof(date), "%c", tm);

        //Write in file
        fprintf(fp, "%s\n", (char*)date);
        fprintf(fp, "%s\n", (char*)message);

        fclose(fp);
    }
}