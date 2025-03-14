#include "mylogger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
