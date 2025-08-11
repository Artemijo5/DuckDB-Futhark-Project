#include "mylogger.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

FILE* loginit(const char* filename, const char* header) {
	if(!filename) return NULL;

	FILE* ptr = (strcmp(filename, "stdout") != 0)? fopen(filename, "a+"): stdout;
	if(!ptr) {
		return NULL;
	}

   	struct timeval now;
   	gettimeofday(&now, NULL);

	fprintf(ptr, "\n\n################################################\n");
	fprintf(ptr, "%s %ldms: %s\n", ctime(&(now.tv_sec)), (now.tv_usec/1000), header);
	fprintf(ptr, "################################################\n\n");
	fflush(ptr);

	return ptr;
}
void mylog(FILE* logfile, const char* msg) {
	if(!logfile) return;

   	struct timeval now;
   	gettimeofday(&now, NULL);

	fprintf(logfile, "%s %ldms : %s\n", ctime(&(now.tv_sec)), (now.tv_usec/1000), msg);
	fflush(logfile);
}
void logdbg(FILE* logfile, int attempt, const char* success, const char* failure) {
	if(!logfile) return;

	if(attempt) mylog(logfile, success);
	else mylog(logfile, failure);
}
void logclose(FILE* logfile) {
	if(!logfile) return;

	mylog(logfile, "Logging session terminated.");
	if (logfile != stdout) fclose(logfile);
}

void logarray_short(FILE* logfile, const char* header, short* arr, int size) {
	if(!logfile) return;

	char res[12*size + strlen(header) + 1];

	int len = sprintf(res, "%s", header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%hd, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_int(FILE* logfile, const char* header, int* arr, int size) {
	if(!logfile) return;

	char res[15*size + strlen(header) + 1];
	
	int len = sprintf(res, "%s", header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%d, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_long(FILE* logfile, const char* header, long* arr, int size) {
	if(!logfile) return;

	char res[25*size + strlen(header) + 1];
	
	int len = sprintf(res, "%s", header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%ld, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_float(FILE* logfile, const char* header, float* arr, int size) {
	if(!logfile) return;

	char res[25*size + strlen(header) + 1];
	
	int len = sprintf(res, "%s", header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%.4f, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_double(FILE* logfile, const char* header, double* arr, int size) {
	if(!logfile) return;

	char res[25*size + strlen(header) + 1];
	
	int len = sprintf(res, "%s", header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%.4f, ", arr[i]);
	}
	mylog(logfile, res);
}