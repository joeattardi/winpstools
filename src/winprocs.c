/**
 * winprocs.c
 * Created January 2, 2005
 * Author: Joe Attardi [jattardi@gmail.com]
 *
 * Contains common functions used by all Windows Process Tools programs.
 * These functions include Win32 API calls to look up and get processes,
 * as well as string/regular expression matching.
 *
 * 
 */
#include "winpstools.h"
#include "winprocs.h"

WINPSTOOLS_OPTIONS getDefaultOptions() {
    WINPSTOOLS_OPTIONS options;
    
    options.matchMode = MATCH_ALL;
    options.output = OUTPUT_STANDARD;
    options.matchCase = TRUE;
    
    return options;
}

PROCESS_SET getProcesses(WINPSTOOLS_OPTIONS options) {
    if (options.matchMode == MATCH_ALL) 
        return getAllProcesses();
    else
        return getProcessesForOptions(options);
}

/**
 * Gets all the currently running processes and packages them in an array of
 * PROCESSENTRY32 objects.
 *
 * Returns:
 *     a PROCESS_SET containing all running processes      
 */
PROCESS_SET getAllProcesses()
{
    /* Get a snapshot of all the running processes. */                
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
    PROCESSENTRY32 pEntry;
   	pEntry.dwSize = sizeof(PROCESSENTRY32);   	   	
   	
    /* Start iterating through processes by fetching the first one. */
	Process32First(snapshot, &pEntry);   
    int processCount = 1;
    
    /* Iterate through the processes to get the number of processes. */
    do
      processCount++;
    while (Process32Next(snapshot, &pEntry));
    
    /* Initialize the list of processes. */
    PROCESSENTRY32* pList = (PROCESSENTRY32*) malloc(sizeof(PROCESSENTRY32) * processCount);
    PROCESS_SET pSet;
    pSet.size = processCount;
    pSet.list = pList;    	
	
    /* Start iterating over again, this time copying the process object into
     * the process list. */
    Process32First(snapshot, &pEntry);
    int i = 0;
    for (i = 0; i < processCount; i++)    
    {
		pList[i] = pEntry;		
        Process32Next(snapshot, &pEntry);
    }
    
    /* Done looking at the snapshot - close its handle */
	CloseHandle(snapshot);    
    
    return pSet;
}

/**
 * Gets processes that match a certain search criteria.
 *
 * Parameters:
 *     searchStr    The search string, or regular expression, to use.
 *     matchCase    Whether or not to match case when searching.
 *     useRegex     Whether or not to use regular expressions. If useRegex
 *                  is false, a simple substring check is used instead.
 *
 * Returns:
 *     a PROCESS_SET containing all matching processes
 */
PROCESS_SET getProcessesForOptions(WINPSTOOLS_OPTIONS options)
{
    /* Get a snapshot of all the running processes. */                
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        
    PROCESSENTRY32 pEntry;
   	pEntry.dwSize = sizeof(PROCESSENTRY32);   	   	
   	
    /* Start iterating through processes by fetching the first one. */
	Process32First(snapshot, &pEntry);   
    int processCount = 1;
    
    /* Iterate through the processes to get the number of processes. */
    do
      processCount++;
    while (Process32Next(snapshot, &pEntry));
    
    /* Initialize the list of processes. */
    PROCESSENTRY32* pList = (PROCESSENTRY32*) malloc(sizeof(PROCESSENTRY32) * processCount);
    PROCESS_SET pSet;                
    
    int numMatches = 0;
    /* Start iterating over again, this time copying the process object into
     * the process list. */
    Process32First(snapshot, &pEntry);
    int i = 0;
    for (i = 0; i < processCount; i++)    
    {                                  
        /* If this process matches the search string, add it to the 
         * result list. */                                       
        if (processMatch(pEntry, options))                     
            pList[numMatches++] = pEntry;
        Process32Next(snapshot, &pEntry);
    }    
    
    /* Done looking at the snapshot - close its handle */
	CloseHandle(snapshot); 

    pSet.list = pList;
    pSet.size = numMatches;    
    
    return pSet;            
}

/**
 * Checks if a given process matches the criteria to be displayed in the 
 * process table output.
 * 
 * Parameters:
 *      pEntry  A PROCESSENTRY32 structure to check.
 * 
 * Returns:
 *      1       if the process is acceptable to display
 *      0       if the process should be omitted
 */
int processMatch(PROCESSENTRY32 pEntry, WINPSTOOLS_OPTIONS options)
{               
    /* If the -n argument was passed, check if the name matches what the
     * user entered. */
    if (options.matchMode == MATCH_NAME && matchName(pEntry.szExeFile, options.queryString, options.matchCase) == FALSE)
        return FALSE;
    
    /* If the -e argument was passed, check if the regular expression matches
     * the process name. */
    if (options.matchMode == MATCH_REGEX && matchRegex(options.queryString, pEntry.szExeFile, options.matchCase) == FALSE)
        return FALSE;
    
    /* All the tests succeeded, so it's OK to display this one. */
    return TRUE;
}

/**
 * Determines if a process name matches a search string.
 *
 * Parameters:
 *      psName      The process name.
 *      searchStr   The string to search for to match.
 *      matchCase   Whether or not to match the case of the strings.
 * Returns:
 *      true        if psName contains searchStr;
 *      false       if not.
 */
int matchName(char* psName, char* searchStr, int matchCase)
{
    /* If the .exe extension was not added to the search name, add it now. */
    if (strstr(searchStr, ".exe") == NULL)
       strcat(searchStr, ".exe");

    if (matchCase)
        return (strcmp(psName, searchStr) == 0);
    else    
        return (_stricmp(psName, searchStr) == 0);    
}

/**
 * Matches a regular expression to a given string.
 * 
 * Parameters:
 *      regex       The regular expression to use.
 *      str         The string to match against.
 *      matchCase   Whether or not to use case-sensitive matching.
 * Returns:
 *      true        if str matches the regular expression,
 *      false       if not.
 */
int matchRegex(char* regex, char* str, int matchCase)
{
    int flags = 0;

    if (!matchCase)
        flags = REG_ICASE;    
       
    /* Compile the regular expression */
    regex_t* regexBuf = (regex_t*) malloc(sizeof(regex_t));        
    /* TODO add error checking with regerror() */
    regcomp(regexBuf, regex, flags);
    
    /* Try to match the string against the regular expression. */
    int result = regexec(regexBuf, str, 0, NULL, flags);        
    
    /* free allocated memory */
    free(regexBuf);
    
    return (result == 0);
}

/**
 * Terminates a process with a given PID.
 * @param pid The PID of the process to be terminated.
 * @return a nonzero value if the process was terminated successfully, or zero if there was an error.
 */
int killProcess(int pid)
{
	HANDLE  process;    // A handle to the process we want to end    
    int     result;     // 0 if there was an error, nonzero if successful
    
    // Get a handle to the process we want to end
    process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);              
          
    if (process == NULL) {        
    	return 0;
    }
    else {
        result = TerminateProcess(process, 0);	    
        CloseHandle(process);                        
    }
    
    return result;
}
