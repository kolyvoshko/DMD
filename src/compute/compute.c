/**
   @file      compute.c
   @brief     Compute module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include "compute.h"
#include "math.h"

void computeDistribution (t_pSys  pSys);
void compute_pe(t_pSys  pSys);
void compute_ke(t_pSys  pSys);

/** \brief Initialize the structure t_compute
 *
 * \param ini Dictionary to search
 * \param key Key structures: program management modules
 * \return Initialized t_compute structure
 *
 */
t_compute compute_init (dictionary *ini, t_key *key){
    t_compute compute;
    if(key->compute_rdf) compute.rdf = init_RDF(ini);
    return compute;
}


/** \brief Run compute modules
 *
 * \param key   Key structures: program management modules
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param compute   This object saves variable compute commands.
 * \return void
 *
 */
void compute_run  (t_key *key, t_pSys *pSys, t_opts *opts, t_compute *compute){
    if (key->compute_temp) if(!(opts->thisIter % opts->snapInterv)) computeTemp(pSys);
    if (key->compute_rdf)  compute->rdf.stepCounter++;
}


/** \brief Run dr submodule compute module
 *
 * \param key   Key structures: program management modules
 * \param compute   This object saves variable compute commands.
 * \param dr    Double distance between two atoms variable
 * \return void
 *
 */
void compute_dr   (t_key *key, t_compute *compute, double *dr){
    if (key->compute_rdf) {
        if(!(compute->rdf.stepCounter % compute->rdf.nStep)){
            t_rdf rdf = compute->rdf;
            compute->rdf = writeRDFarray(&rdf, dr);
        }
    }
}


/** \brief Calculate system temperature
 *
 * \param pSys  System object
 * \return void
 *
 */
void computeTemp(t_pSys *pSys){
    pSys->temperature = (2*pSys->eKin * EKIN_CONST)/(pSys->dim * pSys->pCnt * KBOLTZ);
}


void computePairForces(t_key *key, t_pSys *pSys, t_pair *p){
    int i, j;
    double force, dx, dy, dz, dr;
    for (i = 0; i < pSys->pCnt; i++){
        for (j = 0; j < pSys->pCnt; j++){
            dx    = pSys->x[i] - pSys->x[j];
            dy    = pSys->y[i] - pSys->y[j];
            dz    = pSys->z[i] - pSys->z[j];
            // Applying periodic boundaries to interparticular distances.
            if(dx >  pSys->lx/2.0) dx -= pSys->lx;
            if(dx < -pSys->lx/2.0) dx += pSys->lx;
            if(dy >  pSys->ly/2.0) dy -= pSys->ly;
            if(dy < -pSys->ly/2.0) dy += pSys->ly;
            if(dz >  pSys->lz/2.0) dz -= pSys->lz;
            if(dz < -pSys->lz/2.0) dz += pSys->lz;

            dr    = sqrt(dx*dx + dy*dy + dz*dz);

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
            }
        }
    }
}

double * computeVelosity(t_pSys  pSys){
    int i;
    double *Velocity;
    Velocity = (double*)malloc(sizeof(double)*pSys.pCnt);
    for (i=0; i<pSys.pCnt; i++){
 //       Velocity[i] = sqrt( (pSys.vx)*(pSys.vx) + (pSys.vy)*(pSys.vy) + (pSys.vz)*(pSys.vz));
    }
    return Velocity;
}

void computeVelosityDistribution (double *vel, int size){
    double vmax = 0, vmin = 1e24, sum = 0, average = 0, distrVal;
    int i = 0, sizeDistr;
    for (i = 0; i < size; i++){
        if (fabs(vel[i]) > vmax) vmax = fabs(vel[i]); // Search maximum velocity
        if (fabs(vel[i]) < vmin) vmin = fabs(vel[i]); // Search minimum velocity
        sum += fabs(vel[i]);
    }

    sizeDistr = (vmax - vmin)*5;

    double *DistributionCnt = (double*)malloc(sizeof(double)*sizeDistr);
    for (i = 0; i < size; i++){
        sizeDistr = 0;
    }
}
