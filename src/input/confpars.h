#ifndef CONFPARS_H_INCLUDED
#define CONFPARS_H_INCLUDED

int  parse_ini_file(char * ini_name, t_key *key, t_pSys *pSys, t_opts *opts, t_pair *p, t_fix *fix, t_compute *compute);
#include "confpars.c"
#endif // CONFPARS_H_INCLUDED
