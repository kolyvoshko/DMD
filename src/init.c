/**
   @file      init.c
   @brief     Initialization module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <math.h>
#include <time.h>
#include "struct.h"
#include "compute/compute.h"
#include "pair/pair.h"

static void setAtomType  (t_pSys *pSys);
static void setInitCoords(t_pSys *pSys);
static void setInitVelocs(t_pSys *pSys, t_opts *opts);

void init (t_key    *key,
           t_pSys   *pSys,
           t_opts   *opts,
           t_pair   *p,
           t_compute *compute)
{
    opts->thisIter   = 0;
    opts->snapInterv = 1000;
    opts->vmax       = 1;

    pSys->lx         = 40;
    pSys->ly         = 40;
    pSys->lz         = 40;

    strcpy(pSys->nameType[0],"He");
    strcpy(pSys->nameType[1],"He");
    pSys->type[0]    = 0;
    pSys->type[1]    = 1;
    pSys->massType[0]= 4.002602;
    pSys->massType[1]= 4.002602;
    pSys->qType[0]   = 0.0;
    pSys->qType[1]   = 0.0;

    p->pair_lj.epsilon[0][0] = 7.610;        p->pair_lj.epsilon[1][1] = 7.610;
    p->pair_lj.sigma[0][0]   = 2.000;        p->pair_lj.sigma[1][1]   = 2.000;

    p->pair_morse.d[0][0]    = 7.610*1.2e-4; p->pair_morse.d[1][1]    = 7.610*1.2e-4;
    p->pair_morse.beta[0][0] = 2.126;        p->pair_morse.beta[1][1] = 2.126;
    p->pair_morse.r0[0][0]   = 2.963;        p->pair_morse.r0[1][1]   = 2.963;
    p->pair_coul.r_cutoff_coul = 5.000;

    setAtomType     (pSys);
    setInitCoords   (pSys);
    setInitVelocs   (pSys, opts);

    if (key->pair)    pair_init(key, p);
}

/** \brief set for all atoms random atom type
 *
 * \param  pSys object saves  system's atoms atom type.
 * \return void
 *
 */
static void setAtomType(t_pSys *pSys){
    int i;
    for(i = 0; i < pSys->pCnt; i++){
        int atmType;
        atmType = rand() % pSys->nType;

        pSys->atomType[i]   = atmType;
        pSys->mass[i]       = pSys->massType[atmType];
        pSys->q[i]          = pSys->qType[atmType];
    }
}

/** \brief set for all atoms coordination
 *
 * \param  pSys object saves coordination of system's atoms  .
 * \return void
 *
 */
static void setInitCoords (t_pSys *pSys){
    int i;
    double xstep = pSys->lx / 5.0,
           ystep = pSys->ly / ((pSys->pCnt / 5) + 1 );

    for(i = 0; i < pSys->pCnt; i++){
        if (pSys->dim > 0) pSys->x[i] = xstep/2.0 + xstep*(i%5);
        if (pSys->dim > 1) pSys->y[i] = ystep/2.0 + ystep*(i/5);
        if (pSys->dim > 2) pSys->z[i] = ystep/2.0 + ystep*(i/5);
    }
}


/** \brief set for all atoms random velocities
 *
 * \param  pSys object saves velocities of system's atoms.
 * \return void
 *
 */
static void setInitVelocs (t_pSys *pSys, t_opts *opts){
    int i;
    double vxbias = 0,
           vybias = 0,
           vzbias = 0;

    srand(time(NULL));
    for(i = 0; i < pSys->pCnt; i++){
        if (pSys->dim > 0) vxbias += pSys->vx[i] = opts->vmax * rand() / RAND_MAX;
        if (pSys->dim > 1) vybias += pSys->vy[i] = opts->vmax * rand() / RAND_MAX;
        if (pSys->dim > 2) vzbias += pSys->vz[i] = opts->vmax * rand() / RAND_MAX;
    }

    vxbias /= pSys->pCnt;
    vybias /= pSys->pCnt;
    vzbias /= pSys->pCnt;

    for(i = 0; i < pSys->pCnt; i++){
        if (pSys->dim > 0) pSys->vx[i] -= vxbias;
        if (pSys->dim > 1) pSys->vy[i] -= vybias;
        if (pSys->dim > 2) pSys->vz[i] -= vzbias;

        pSys->eKin += pSys->mass[i] * ( pSys->vx[i]*pSys->vx[i] +
                                        pSys->vy[i]*pSys->vy[i] +
                                        pSys->vz[i]*pSys->vz[i]   ) / 2.0;
    }
}
