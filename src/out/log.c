/**
   @file      log.c
   @brief     create .log file
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 9 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

#define LOGFILE	"mdRun.log" /**< all ReportMessage(); messages will be appended to this file */
int LogCreated = 0;

enum ConsoleColor {
        Black = 0,
        Blue = 1,
        Green = 2,
        Cyan = 3,
        Red = 4,
        Magenta = 5,
        Brown = 6,
        LightGray = 7,
        DarkGray = 8,
        LightBlue = 9,
        LightGreen = 10,
        LightCyan = 11,
        LightRed = 12,
        LightMagenta = 13,
        Yellow = 14,
        White = 15,
        Default = 15
};


/** \brief print message to stdout and to log file */
void printLog (const char *format, va_list args)
{
	FILE *file;

	if (LogCreated == 0) {
		file = fopen(LOGFILE, "w");
		LogCreated = 1;
	}
	else
		file = fopen(LOGFILE, "a");

	if (file == NULL) {
		if (LogCreated == 1)
			LogCreated = 0;
		return;
	}
	else
	{
		vfprintf(file, format, (va_list)args);
		fclose(file);
	}

	if (file)
		fclose(file);
}


/** \brief print error message to to log file */
void printErrorLog (const char *format, va_list args)
{
	FILE *file;

	if (LogCreated == 0) {
		file = fopen(LOGFILE, "w");
		LogCreated = 1;
	}
	else
		file = fopen(LOGFILE, "a");

	if (file == NULL) {
		if (LogCreated == 1)
			LogCreated = 0;
		return;
	}
	else
	{
		vfprintf(file, format, (va_list)args);
		fclose(file);
	}

	if (file)
		fclose(file);
}

/** \brief Report from Error message to stderr and to log file */
void ReportErrorMesage (const char *format, ...)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD) (LightRed));
    va_list args;
    va_start( args, format );
    vfprintf(stderr, format, (va_list)args);
	printErrorLog(format, args);
	va_end(args);
	SetConsoleTextAttribute(hConsole, (WORD) (Default));
	exit(EXIT_FAILURE);
}

/** \brief Report from Warning message to stderr and to log file */
void ReportWarningMessage (const char *format, ...){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD) (Yellow));
    va_list args;
    va_start( args, format );
    vfprintf(stderr, format, (va_list)args);
    printErrorLog(format, args);
    va_end(args);
	SetConsoleTextAttribute(hConsole, (WORD) (Default));
}

/** \brief Report from message to stderr and to log file */
void ReportMessage (const char *format, ...){
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (WORD) (Default));
    va_list args;
    va_start( args, format );
    vprintf(format, (va_list)args);
    printLog(format, args);
    va_end(args);
	SetConsoleTextAttribute(hConsole, (WORD) (Default));
}
