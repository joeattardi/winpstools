/**
 * winprocs.h
 * Created January 2, 2005, 10:03 pm
 * Author: Joe Attardi [joe@thinksincode.net]
 *
 * Contains definitions for common functions used by all Windows Process 
 * Tools programs.
 * These functions include Win32 API calls to look up and get processes,
 * as well as string/regular expression matching.
 *
 * 
 */
#ifndef _WINPROCS_H
#define _WINPROCS_H
#endif

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <sys/types.h>
#include "regex.h"

#define MATCH_ALL 0
#define MATCH_NAME 1
#define MATCH_REGEX 2

#define OUTPUT_STANDARD 0
#define OUTPUT_PID_ONLY 1

typedef struct processSet {
    int size;
    PROCESSENTRY32* list;	
} PROCESS_SET;

typedef struct winpstoolsOptions {
    int   matchMode;
    int   matchCase;
    int   output;  
    int   pid;
    char* queryString;
} WINPSTOOLS_OPTIONS;

WINPSTOOLS_OPTIONS getDefaultOptions();
PROCESS_SET getProcesses(WINPSTOOLS_OPTIONS);
PROCESS_SET getAllProcesses();
PROCESS_SET getProcessesForOptions(WINPSTOOLS_OPTIONS);
int processMatch(PROCESSENTRY32, WINPSTOOLS_OPTIONS);
int matchName(char* psName, char* searchStr, int matchCase);
int matchRegex(char* regex, char* str, int matchCase);
int killProcess(int pid);
