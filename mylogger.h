#include <stdio.h>

#ifndef MYLOGGER_H
#define MYLOGGER_H

/* A barebones logger with some basic functions for documenting progress. */

FILE* loginit(const char* filename, const char* header);
void mylog(FILE* logfile, const char* msg);
void logdbg(FILE* logfile, int attempt, const char* success, const char* failure);
void logclose(FILE* logfile);

void logarray_short(FILE* logfile, const char* header, short* arr, int size);
void logarray_int(FILE* logfile, const char* header, int* arr, int size);
void logarray_long(FILE* logfile, const char* header, long* arr, int size);
void logarray_float(FILE* logfile, const char* header, float* arr, int size);
void logarray_double(FILE* logfile, const char* header, double* arr, int size);

#endif
