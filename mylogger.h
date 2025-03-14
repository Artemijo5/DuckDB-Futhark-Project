#include <stdio.h>

#ifndef MYLOGGER_H
#define MYLOGGER_H

/* A barebones logger with some basic functions for documenting progress. */

FILE* loginit(const char* filename, const char* header);
void mylog(FILE* logfile, const char* msg);
void logdbg(FILE* logfile, int attempt, const char* success, const char* failure);
void logclose(FILE* logfile);

#endif
