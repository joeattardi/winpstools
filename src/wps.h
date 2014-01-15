/**
 * wps
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
#ifndef _WPS_H
#define _WPS_H

#include <stdio.h>
#include <unistd.h>
#include "winprocs.h"
#include "winpstools.h"
#include "wps_resources.h"

void printHeading(int);
WINPSTOOLS_OPTIONS processArgs(int, char**);
void printVersionInfo();
void printOptions();

#endif
