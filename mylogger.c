#include "mylogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

FILE* loginit(const char* filename, const char* header) {
	FILE* ptr = fopen(filename, "a+");
	if(ptr == NULL) {
		return NULL;
	}

	time_t now;
   	time(&now);

	fprintf(ptr, "\n\n################################################\n");
	fprintf(ptr, "%s : %s", ctime(&now), header);
	fprintf(ptr, "\n################################################\n\n");
	fflush(ptr);

	return ptr;
}
void mylog(FILE* logfile, const char* msg) {
	time_t now;
   	time(&now);

	fprintf(logfile, "%s : %s\n", ctime(&now), msg);
	fflush(logfile);
}
void logdbg(FILE* logfile, int attempt, const char* success, const char* failure) {
	if(attempt) mylog(logfile, success);
	else mylog(logfile, failure);
}
void logclose(FILE* logfile) {
	mylog(logfile, "Logging session terminated.");
	fclose(logfile);
}

void logarray_short(FILE* logfile, const char* header, short* arr, int size) {
	char res[12*size + strlen(header) + 1];

	int len = sprintf(res, header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%hd, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_int(FILE* logfile, const char* header, int* arr, int size) {
	char res[12*size + strlen(header) + 1];
	
	int len = sprintf(res, header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%d, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_long(FILE* logfile, const char* header, long* arr, int size) {
	char res[12*size + strlen(header) + 1];
	
	int len = sprintf(res, header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%ld, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_float(FILE* logfile, const char* header, float* arr, int size) {
	char res[17*size + strlen(header) + 1];
	
	int len = sprintf(res, header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%.4f, ", arr[i]);
	}
	mylog(logfile, res);
}
void logarray_double(FILE* logfile, const char* header, double* arr, int size) {
	char res[17*size + strlen(header) + 1];
	
	int len = sprintf(res, header);
	for(int i=0; i<size; i++) {
		len += sprintf(res + len, "%.4f, ", arr[i]);
	}
	mylog(logfile, res);
}