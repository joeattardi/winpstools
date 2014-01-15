/**
 * WinPS 1.5
 * A Windows process viewer
 * by Joe Attardi [joe@thinksincode.net]
 *
 * Reports information about running Windows processes. 
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
#include "wps.h"

int main(int argc, char** argv)
{
	/* See if we have command-line arguments */
    WINPSTOOLS_OPTIONS options = processArgs(argc, argv);

    /* Display the column headings for the process table. */
	printHeading(options.output);
 
    PROCESS_SET pSet;
    PROCESSENTRY32 pEntry;
    int pIndex = 0;
  
    pSet = getProcesses(options);

    for (pIndex = 0; pIndex < pSet.size; pIndex++)
    {
        pEntry = pSet.list[pIndex];  
              
        if (options.output == OUTPUT_STANDARD)
            printf("\t%d\t%d\t%d\t%s\t\n", 
                  (int)pEntry.th32ProcessID, (int)pEntry.th32ParentProcessID,
                  (int)pEntry.pcPriClassBase, pEntry.szExeFile);				  
        else if (options.output == OUTPUT_PID_ONLY)
            printf("%d\n", (int)pEntry.th32ProcessID);
        }

	return 0;
}

/**
 * Processes command-line arguments to the program.
 *
 * Parameters:
 *      argc    The number of command-line arguments.
 *      argv    The argv array of command-line arguments.
 */
WINPSTOOLS_OPTIONS processArgs(int argc, char** argv)
{
    WINPSTOOLS_OPTIONS options = getDefaultOptions();
    char argChar;    
    
    while ((argChar = getopt(argc, argv, "vin:e:p?")) != -1) {
        switch (argChar) {
            case 'v':
                printVersionInfo();
                exit(0);
            case 'i':
                options.matchCase = FALSE;
                break;            
            case 'n':
                options.matchMode = MATCH_NAME;
                options.queryString = optarg;
                break;
            case 'e':
                options.matchMode = MATCH_REGEX;
                options.queryString = optarg;
                break;
            case 'p':
                options.output = OUTPUT_PID_ONLY;
                break;
            case '?':
                printOptions(); 
                exit(1);
        }
    }
    
    return options;
}

/**
 * Prints some version information.
 */
void printVersionInfo()
{
    TCHAR stringBuffer[256];
    HINSTANCE appHandle = GetModuleHandle(NULL);    
    LoadString(appHandle, COPYRIGHT_STATEMENT, stringBuffer, sizeof(stringBuffer) / sizeof(TCHAR));    
    printf(stringBuffer, VERSION, __DATE__);    	
}

/**
 * Prints a listing of all program command-line options.
 */
void printOptions()
{
	printVersionInfo();
	printf("Command line options are:\n\n");
	printf("-?          Show help (this screen)\n");
    printf("-e <regex>  Find processes using a regular expression\n");
	printf("-i          Ignore case when using -n option\n");
	printf("-n <name>   Only show processes whose commands match <name>\n");
	printf("-p          Only display process ID (PID); omit all other information\n");
	printf("-v          Show version information\n\n");
	printf("\n");
	printf("NOTE: The -n option will only match a process name in its entirety \n");
	printf("(with or without the .exe extension). If you want to match part of \n");
	printf("a name, use the -e option.\n\n");	
}

/**
 * Prints the heading of the process list. This is repeated
 * every 20 lines for listings that extend off the page.
 */
void printHeading(int outputFlag)
{
	/* Print out the heading for the process table.	 */
	if (outputFlag == OUTPUT_STANDARD)
	    printf("\tPID\tPPID\tPRI\tCOMMAND\n");	
}

