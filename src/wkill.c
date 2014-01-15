/**
 * WinKill 1.5
 * A Windows process killer
 * by Joe Attardi (joe@thinksincode.net)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "wkill.h"

int main(int argc, char** argv)
{
	int n_pid;      // The PID of the process we want to end
	int result;
	
    WINPSTOOLS_OPTIONS options;
    
	// Make sure a process ID was passed.
	if (argc < 2)
	{
		printOptions();
		return 1;
	}
	else
	    options = processArgs(argc, argv);

    if (options.matchMode != MATCH_ALL)
	{        
        PROCESSENTRY32 pEntry;
        int pIndex = 0;
 
        PROCESS_SET pSet = getProcesses(options);

        for (pIndex = 0; pIndex < pSet.size; pIndex++)
        {
            pEntry = pSet.list[pIndex];  
			n_pid = (int)pEntry.th32ProcessID;
            result = killProcess(n_pid);	
			
			if (result == 0)	
				handleError(n_pid);
			else
				printf("PID %d (%s) terminated successfully.\n", n_pid, pEntry.szExeFile);					
        }
    }
    else
   	{               
	    result = killProcess(options.pid);
		
		if (result == 0)	
			handleError(options.pid);
		else
			printf("PID %d terminated successfully.\n", n_pid);							
    }	    

	return 0;
}

/**
 * Prints some version information.
 */
void printVersionInfo()
{
	printf("wkill %s (%s), Copyright (C) 2005 by Joe Attardi [joe@thinksincode.net]\n", VERSION, __DATE__);
	printf("Kills Windows processes\n\n");
	printf("wkill is free software distributed under the GNU General Public License.\n\n");
}

WINPSTOOLS_OPTIONS processArgs(int argc, char** argv)
{
    WINPSTOOLS_OPTIONS options = getDefaultOptions();
    char argChar;
    
    while ( (argChar = getopt(argc, argv, "vin:e:?")) != -1) {
        switch (argChar) {
        
            case 'v':	// print version info
                printVersionInfo();
                exit(0);            
                
            case 'i': 	// case insensitivity
                options.matchCase = FALSE;
                break;
                
            case 'n':	// match process name
                options.matchMode = MATCH_NAME;                
                options.queryString = optarg;
                break;
                
            case 'e':   // match a regular expression
                options.matchMode = MATCH_REGEX;
                options.queryString = optarg;
                break;
                
            case '?':	// list command-line options
                printOptions();
                exit(1);
        }
    }
    
    // If the PID was passed on its own, pick that up.
    if (optind < argc) {
        options.pid = atoi(argv[optind]);        
    }
    
    return options;
}

/**
 * Handles any errors that occurred while trying to kill the process.
 *
 * @param errorCode The error code returned by the call to TerminateProcess().
 * @param n_pid     The PID we were trying to terminate.
 */
void handleError(int n_pid)
{    
    const char* errorString = "wkill: Unable to terminate PID %d: %s\n";
    char* message;
    int errorCode = GetLastError();        
 
	#ifdef DEBUG
		printf("Caught error %d.\n", errorCode);
	#endif      
    
	// Print an appropriate error message.
	switch (errorCode)
	{
		case ERROR_INVALID_PARAMETER:
            message = "No such PID.";			
            break;
		case ERROR_ACCESS_DENIED:
            message = "Not permitted to terminate this PID.";						
            break;
	}
        
    fprintf(stderr, errorString, n_pid, message);	
}

/**
 * @function printOptions
 * Prints a listing of all program command-line options.
 */
void printOptions()
{
	printVersionInfo();

    printf("Usage: \n");
    printf(" wkill <PID>                (kill a specific process ID)\n");
    printf(" or wkill -n <name> [options]  (kill all processes matching name)\n");
    printf(" or wkill -e <regex> [options] (kill all processes matching regular expression)\n");
    printf("Available options:\n");
	printf(" -i  Ignore case when using -n or -e options\n");
	printf("\n");
	printf("NOTE: The -n option will only match a process name in its entirety \n");
	printf("(with or without the .exe extension). If you want to match part of \n");
	printf("a name, use the -e option.\n\n");
	
}
