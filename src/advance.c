/**
   @file      advance.c
   @brief     Advance module: run advance style modeling.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <math.h>

#include "out/out.h"
#include "fix/fix.h"
#include "init.h"
#define LOGFILE	"mdRun.log" /**< all Log(); messages will be appended to this file */

static void advance_iter(t_key      *key,
                 t_pSys     *pSys,
                 t_opts     *opts,
                 t_pair     *p,
                 t_fix      *fix,
                 t_compute  *compute);

static void setNewAccels(
                 t_key      *key,
                 t_pSys     *pSys,
                 t_pair     *p,
                 t_compute  *compute);

static void setNewCVA  (
                 t_key      *key,
                 t_pSys     *pSys,
                 t_pair     *p,
                 t_compute  *compute,
                 double     dt);

static void resetStep  (t_pSys     *pSys);



void advance (const char* filename)
{
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
        advance_iter (&key, &pSys, &opts, &p, &fix, &compute);
        if(key.dump) dump_run(&key, &pSys, &opts, &dump);
        resetStep(&pSys);
    }
}


/** \brief  run advance core*/
static void advance_iter (t_key *key, t_pSys *pSys, t_opts *opts, t_pair *p, t_fix *fix, t_compute *compute){
    setNewCVA(key, pSys, p, compute, opts->dt);

    if(key->compute) compute_run(key, pSys, opts, compute);
    if(key->fix)     fix_run (key, pSys, opts, fix);

    opts->thisIter++;
}

/** \brief  compute new acceleration*/
static void setNewAccels (t_key *key, t_pSys *pSys, t_pair *p, t_compute *compute){
    int i, j;
    double force, dx, dy, dz, dr;

    for(i = 0; i < pSys->pCnt; i++){
        pSys->ax[i] = pSys->ay[i] = pSys->az[i] = 0;
    }

    for(i = 0; i < pSys->pCnt - 1; i++){
        for(j = i + 1; j < pSys->pCnt; j++){

            dx    = pSys->x[i] - pSys->x[j];
            dy    = pSys->y[i] - pSys->y[j];
            dz    = pSys->z[i] - pSys->z[j];

            /*
            // Applying periodic boundaries to interparticular distances.
            if(dx >  pSys->lx/2.0) dx -= pSys->lx;
            if(dx < -pSys->lx/2.0) dx += pSys->lx;
            if(dy >  pSys->ly/2.0) dy -= pSys->ly;
            if(dy < -pSys->ly/2.0) dy += pSys->ly;
            if(dz >  pSys->lz/2.0) dz -= pSys->lz;
            if(dz < -pSys->lz/2.0) dz += pSys->lz;*/

            dr    = sqrt(dx*dx + dy*dy + dz*dz);
            if (key->compute_dr) compute_dr(key, compute, &dr);

            if (dr < p->neighbor) {
                p->dr = dr;
                p->force = 0;
                p->epair = 0;
                p->pair_coul.eCoulumb = 0;
                if(key->pair == 1){
                    p->ii = pSys->atomType[i];
                    p->jj = pSys->atomType[j];
                    pair(key, pSys, p);
                }

                pSys->ePot += p->epair + p->pair_coul.eCoulumb;
                force = p->force;

                pSys->ax[i] += force*dx/dr/pSys->mass[i]; // Applying Newton's 3rd law.
                pSys->ay[i] += force*dy/dr/pSys->mass[i];
                pSys->az[i] += force*dz/dr/pSys->mass[i];
                pSys->ax[j] -= force*dx/dr/pSys->mass[j];
                pSys->ay[j] -= force*dy/dr/pSys->mass[j];
                pSys->az[j] -= force*dz/dr/pSys->mass[j];
            }
        }
    }
}


/** \brief  compute new coordinations, velocities and acceleration*/
static void setNewCVA (t_key *key, t_pSys *pSys, t_pair *p, t_compute *compute, double dt){
    int i;
    for(i = 0; i < pSys->pCnt; i++){
        pSys->x[i] += pSys->vx[i]*dt + pSys->ax[i]*dt*dt/2.0;
        pSys->y[i] += pSys->vy[i]*dt + pSys->ay[i]*dt*dt/2.0;
        pSys->z[i] += pSys->vz[i]*dt + pSys->az[i]*dt*dt/2.0;
        // Applying periodic boundaries.
        if(pSys->x[i] < 0)          pSys->x[i] += pSys->lx;
        if(pSys->x[i] > pSys->lx)   pSys->x[i] -= pSys->lx;
        if(pSys->y[i] < 0)          pSys->y[i] += pSys->ly;
        if(pSys->y[i] > pSys->ly)   pSys->y[i] -= pSys->ly;
        if(pSys->z[i] < 0)          pSys->z[i] += pSys->lz;
        if(pSys->z[i] > pSys->lz)   pSys->z[i] -= pSys->lz;
    }
    // Modifying velocities with previous accelerations.
    for(i = 0; i < pSys->pCnt; i++){
        pSys->vx[i] += pSys->ax[i]*dt/2.0;
        pSys->vy[i] += pSys->ay[i]*dt/2.0;
        pSys->vz[i] += pSys->az[i]*dt/2.0;
    }

    setNewAccels(key, pSys, p, compute);
    // Modifying velocities with new accelerations.
    for(i = 0; i < pSys->pCnt; i++){
        pSys->vx[i] += pSys->ax[i]*dt/2.0;
        pSys->vy[i] += pSys->ay[i]*dt/2.0;
        pSys->vz[i] += pSys->az[i]*dt/2.0;
        // Calculating kinetic energy.
        pSys->eKin += pSys->mass[i] * ( pSys->vx[i]*pSys->vx[i] +
                                        pSys->vy[i]*pSys->vy[i] +
                                        pSys->vz[i]*pSys->vz[i] )/2.0;
    }
}


/** \brief reset step*/
static void resetStep (t_pSys *pSys){
    pSys->ePot = 0;
    pSys->eKin = 0;
}
