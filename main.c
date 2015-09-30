/**
   @file      main.c
   @brief     Molecular modeling program.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

/** \mainpage EMD - Easy Molecular Dynamic Software

   \section intro_sec Introduction

   The program for molecular dynamics simulations of simple systems.

   \subsection what_sub What is EMD

   \section install_sec Installation


   @section License

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of
   the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   General Public License for more details at
   https://www.gnu.org/copyleft/gpl.html

   \page Commands
   This section describes how a EMD input script is formatted and the input script
   commands used to define a EMD simulation.

   \subsection conf_sub EMD configuration file
   \subsection comand_list Commands listed by category
    - Compute
     - pe
     - ke
     - te
     - temp
    - Fix
     - thermostat Berensden
     - thermostat Rescale
    - Dump
     - dump file
     - dump image
    - Individual commands
     - print


   \subsection individ_com Individual commands

 */

#include "src/advance.h"
#include "src/verlet.h"
#include "src/out/out.h"

/**
 * \param argv[1] Input filename
 */
int main(int argc, char * argv[]){
    const char* filename = NULL;

    // Get filename from the command line.
    if (argc != 2) {
        filename = "input/config.ini";
        ReportWarningMessage("WARNING: Specify the input file from the command line.\n");
        ReportMessage("Load default configuration file \"%s\":\n\n", filename);
    }
    else{
        filename = argv[1];
        ReportMessage("Load configuration file \"%s\":\n\n", filename);
    }

    advance(filename);

    return 0;
}
