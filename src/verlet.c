/**
   @file      verlet.c
   @brief     Verlet module: run verlet style modeling.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   0.0
   @copyright GNU Public License.
*/


#include <math.h>

#include "out/out.h"
#include "fix/fix.h"
#include "init.h"
#define LOGFILE	"mdRun.log" /**< all Log(); messages will be appended to this file */

static void verlet_iter(t_key      *key,
                         t_pSys     *pSys,
                         t_opts     *opts,
                         t_pair     *p,
                         t_fix      *fix,
                         t_compute  *compute);
static void resetStep   (t_pSys     *pSys);


void verlet(const char* filename){
dictionary* ini;
    ini = iniparser_load(filename);

    iniparser_dump(ini, stdout);
    FILE *file;
    file = fopen(LOGFILE, "a");
    iniparser_dump(ini, file);
    fclose(file);
    ReportMessage("\n");

    t_key       key  = key_init(ini);
    t_pSys      pSys = pSys_init (ini);
    t_opts      opts = opts_init (ini);
    t_pair      p    = t_pair_init(ini);
    t_dump      dump = dump_init(ini);
    t_fix       fix  = fix_init(ini);
    t_compute   compute = compute_init(ini, &key);

    init (&key, &pSys, &opts, &p, &compute);
    if(key.dump) dump_run(&key, &pSys, &opts, &dump); // Make initial snapshot.
    while(opts.thisIter < opts.targIters){  // Repeat until iterational limit.
        verlet_iter (&key, &pSys, &opts, &p, &fix, &compute);
        if(key.dump) dump_run(&key, &pSys, &opts, &dump);
        resetStep(&pSys);
    }
}


static void verlet_iter(t_key      *key,
                         t_pSys     *pSys,
                         t_opts     *opts,
                         t_pair     *p,
                         t_fix      *fix,
                         t_compute  *compute)
{

}

/** \brief reset step*/
static void resetStep (t_pSys *pSys){
    pSys->ePot = 0;
    pSys->eKin = 0;
}
