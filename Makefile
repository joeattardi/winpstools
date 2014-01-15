#---------------------------------------------------
# Makefile for Windows Process Tools  
#---------------------------------------------------

#------------------------------------------------------------------
# Tool paths: 
# Change these to point to where you have these tools installed.                            
#------------------------------------------------------------------
GCC=c:\MinGW\bin\gcc.exe
GPP=c:\MinGW\bin\g++.exe
WINDRES=C:\MinGW\bin\windres.exe
RM=C:\cygwin\bin\rm.exe

#---------------------------------------------------
# Change the INSTALL_DIR variable to the directory
# you want to install WinPSTools to.
#---------------------------------------------------
INSTALL_DIR=C:\WINPSTOOLS

#---------------------------------------------------
# You shouldn't need to change anything below this
# line, so don't - unless you know what you're doing!
#---------------------------------------------------
SRC_DIR=src
GCC_OPTS=-Wall


all: wps.exe wkill.exe

wps.exe: ${SRC_DIR}\wps.c ${SRC_DIR}\wps.h winpstools.dll wps_resources.o	
	${GPP} ${SRC_DIR}\wps.c wps_resources.o ${GCC_OPTS} -L./ -lwinpstools -o wps.exe

wps_resources.o: ${SRC_DIR}\wps.rc
	${WINDRES} ${SRC_DIR}\wps.rc wps_resources.o
    
wkill.exe: ${SRC_DIR}\wkill.c winpstools.dll
	${GPP} ${SRC_DIR}\wkill.c ${GCC_OPTS} -L./ -lwinpstools -o wkill.exe

winpstools.dll: ${SRC_DIR}\winprocs.c 
	${GPP} ${SRC_DIR}\winprocs.c ${GCC_OPTS} -L./lib -lgnu_regex -shared -o winpstools.dll

install: all
	install.bat ${INSTALL_DIR}
clean:
	${RM} -f winpstools.dll
	${RM} -f *.exe
	${RM} -f *.o
	${RM} -f *.zip
	${RM} -rf dist
	${RM} -rf src-dist


