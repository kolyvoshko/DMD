#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "confpars.h"
#include <windows.h>

int  parse_ini_file(char * ini_name, t_key *key, t_pSys *pSys, t_opts *opts, t_pair *p, t_fix *fix, t_compute *compute){
    _TCHAR c[10];

    /// System
    pSys->dim   = GetPrivateProfileInt("System", "Dimension", 3, ini_name);
    p->neighbor = GetPrivateProfileInt("System", "Neighbor", 5, ini_name);

    /// Time
    opts->targIters = GetPrivateProfileInt("Time", "Run", 100000, ini_name);
    opts->dt        = GetPrivateProfileInt("Time", "TimeStep", 1e-3, ini_name);

    /// Data

    /// Potential
    GetPrivateProfileString("Potential", "Included", "OFF", c, sizeof(c) / sizeof(c[0]), ini_name);
    if (strcmp(c, "ON") == 0 ) key->pair = 1;
    else key->pair = 0;

    /// Termostat
    GetPrivateProfileString("Termostat", "Included", "OFF", c, sizeof(c) / sizeof(c[0]), ini_name);
    if (strcmp(c, "ON") == 0 ) key->term = 1;
    else key->term = 0;


    //Berensden

    // Rescale




    return 0;
}
