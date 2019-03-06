/*
 * File: Common.h
 * Authors: Raul Arias Quesada - Bryan Abarca Weber - Rony Panigua Chacon
 * Description: Header file for common methods
 */

#ifndef SERVER_COMMON_H
#define SERVER_COMMON_H

/*
 * Method for: get port from config file
 * Return: port from config file, if doesnt find the port return 8081
 */
int getPort();

/*
 * Method for: get path from config file
 * Return: log path from config file, if doesnt find the path set /var/log/syslog
 */
char* getLogPath();

/*
 * Method for: write to a file
 * Params message: Message to write
 *        file: Full file path
 */
void writeFile(char* message, char* file);

#endif //SERVER_COMMON_H