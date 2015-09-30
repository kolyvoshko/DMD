/**
   @file      pair.c
   @brief     Pair module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pair.h"
#include "../out/out.h"

#define THISFILE  "pair.c"

static double ** ConstructDouble2d (int nType);
static void      pair_LJ(t_pair *p);
static void      pair_LJ_cutoff(t_pair *p);
static void      pair_Morse(t_pair *p);
static void      pair_Morse_cutoff(t_pair *p);
static void      pair_init_Morse(t_pair *p);
static void      pair_init_LJ(t_pair *p);
static void      pair_coul(t_pSys *pSys, t_pair *p);
static void      pair_coul_cut(t_pSys *pSys, t_pair *p);
static void      Construct_t_pair (t_pair *p, int nType);


/** \brief Initialize the structure t_pair
 *
 * \param ini Dictionary to search
 * \return Initialized t_pair structure
 *
 */
t_pair t_pair_init(dictionary* ini){
    char *k, *k2, *k3, *f;
    int n = 1;
    t_pair p;

    // [Data]
    k = iniparser_getstring(ini, "Data:Mode", "Set");
    if (strcmp(k, "Set") == 0){
        n = iniparser_getint(ini, "Data:Ntype",  2);
        if (n < 1) ReportErrorMesage("ERROR: Ntype must be natural number: src/pair/pair.c t_pair_init");
    }
    if (strcmp(k, "File") == 0){;}

    Construct_t_pair (&p, n);

    k2 = iniparser_getstring(ini, "Potential:Cut", "ON");
    k3 = iniparser_getstring(ini, "Potential:Type", "MORSE");

    p.neighbor = iniparser_getdouble(ini, "System:Neighbor", 6);
    if (p.neighbor < 0.5) ReportErrorMesage("ERROR: neighbor is small (%f): src/pair/pair.c t_pair_init" , p.neighbor);

    f = iniparser_getstring(ini, "Potential:Paramets", "input/morse.he");
    //if (strcmp(k3, "MORSE") == 0 ) {}
    //if (strcmp(k3, "LJ") == 0 ) {}

    return p;
}


void pair_init (t_key *key, t_pair *p){
    if(key->pair_lj || key->pair_lj_cut)       pair_init_LJ    (p);
    if(key->pair_morse || key->pair_morse_cut) pair_init_Morse (p);
}


void pair(t_key *key, t_pSys *pSys, t_pair *p){
    if (key->pair_morse_cut) pair_Morse_cutoff(p);
    if (key->pair_lj_cut)    pair_LJ_cutoff(p);
    if (key->pair_morse)     pair_Morse(p);
    if (key->pair_lj)        pair_LJ(p);
    if (key->pair_coulumb)   pair_coul_cut(pSys, p);
}


/** \brief Alocated memory for double 2D array N * N
 *
 * \param nType Number types of atoms
 * \return void
 *
 */
static double ** ConstructDouble2d (int nType){
    int i;
    double **arr = (double **)malloc(nType * sizeof(double *));
    for (i=0; i<nType; i++)
         arr[i] = (double *)malloc(nType * sizeof(double));
    return arr;
}


/** \brief Initialize t_pair structure
 *
 * \param p     Structure which will be allocated memory
 * \param nType Number types of atoms
 * \return void
 *
 */
static void Construct_t_pair (t_pair *p, int nType){
    p->nType = nType;

    p->pair_morse.beta = ConstructDouble2d(nType);
    p->pair_morse.d    = ConstructDouble2d(nType);
    p->pair_morse.r0   = ConstructDouble2d(nType);

    p->pair_morse.e_morse_cutoff = ConstructDouble2d(nType);
    p->pair_morse.r_morse_cutoff = ConstructDouble2d(nType);

    p->pair_lj.epsilon      = ConstructDouble2d(nType);
    p->pair_lj.sigma        = ConstructDouble2d(nType);
    p->pair_lj.r_cutoff_lj  = ConstructDouble2d(nType);
    p->pair_lj.e_cutoff_lj  = ConstructDouble2d(nType);
}


/** \brief Initialize Leonard-Jones interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Leonard-Jones constant matrix
 */
static void pair_init_LJ(t_pair *p){
    int i;
    for (i=0; i < p->nType; i++) {
        p->pair_lj.r_cutoff_lj[i][i] = 2.5 * p->pair_lj.sigma[i][i];
    }

    int ii;
    for(ii=0; ii < p->nType; ii++){
        int jj;
        for(jj=0; jj < p->nType; jj++){
            if(ii!=jj){
                p->pair_lj.epsilon[ii][jj]         = (p->pair_lj.epsilon[ii][ii]   + p->pair_lj.epsilon[jj][jj])/2;
                p->pair_lj.sigma[ii][jj]           = (p->pair_lj.sigma[ii][ii]     + p->pair_lj.sigma[jj][jj])  /2;

                p->pair_lj.r_cutoff_lj[ii][jj]     = (p->pair_lj.r_cutoff_lj[ii][ii] + p->pair_lj.r_cutoff_lj[jj][jj]) /2;
                p->pair_lj.e_cutoff_lj[ii][jj]     = 4*p->pair_lj.epsilon[ii][jj]  * pow(p->pair_lj.sigma[ii][jj] /p->pair_lj.r_cutoff_lj[ii][jj],6)
                                            *( pow(p->pair_lj.sigma[ii][jj]/p->pair_lj.r_cutoff_lj[ii][jj],6) - 1 );
            }
            p->pair_lj.e_cutoff_lj[ii][ii]  = 4*p->pair_lj.epsilon[ii][ii]  * pow(p->pair_lj.sigma[ii][ii] /p->pair_lj.r_cutoff_lj[ii][ii],6)
                                            *( pow(p->pair_lj.sigma[ii][ii]/p->pair_lj.r_cutoff_lj[ii][ii],6) - 1 );
        }
    }
}


/** \brief Initialize Morse interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Morse constant matrix
 */
static void pair_init_Morse(t_pair *p){
    int i;
    for (i=0; i < p->nType; i++) {
    p->pair_morse.r_morse_cutoff[i][i] = 2.0 * p->pair_morse.r0[i][i];
    }

    int ii;
    for(ii=0; ii < p->nType; ii++){
        int jj;
        for(jj=0; jj < p->nType; jj++){
            if(ii!=jj){
                p->pair_morse.d[ii][jj]                = (p->pair_morse.d[ii][ii]     + p->pair_morse.d[jj][jj])     /2;
                p->pair_morse.beta[ii][jj]             = (p->pair_morse.beta[ii][ii]  + p->pair_morse.beta[jj][jj])  /2;
                p->pair_morse.r0[ii][jj]               = (p->pair_morse.r0[ii][ii]    + p->pair_morse.r0[jj][jj])    /2;

                p->pair_morse.r_morse_cutoff[ii][jj]   = (p->pair_morse.r_morse_cutoff[ii][ii] + p->pair_morse.r_morse_cutoff[jj][jj])/2;
                p->pair_morse.e_morse_cutoff[ii][jj]   =  p->pair_morse.d[ii][jj]*exp(-p->pair_morse.beta[ii][jj]*(p->pair_morse.r_morse_cutoff[ii][jj] - p->pair_morse.r0[ii][jj]))
                                              *(exp(-p->pair_morse.beta[ii][jj]*(p->pair_morse.r_morse_cutoff[ii][jj] - p->pair_morse.r0[ii][jj])) - 2);
            }
            p->pair_morse.e_morse_cutoff[ii][ii]       =  p->pair_morse.d[ii][ii]*exp(-p->pair_morse.beta[ii][ii]*(p->pair_morse.r_morse_cutoff[ii][ii]
                                                                                                                   - p->pair_morse.r0[ii][ii]))
                                              *(exp(-p->pair_morse.beta[ii][ii]*(p->pair_morse.r_morse_cutoff[ii][ii] - p->pair_morse.r0[ii][ii])) - 2);
        }
    }
}


/** \brief Initialize Leonard-Jones interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Leonard-Jones constant matrix
 */
static void pair_LJ(t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;

    double dr6;
    dr6 = pow(p->pair_lj.sigma[ii][jj]/p->dr,6);

    p->epair +=  4*p->pair_lj.epsilon[ii][jj] * dr6       * ( dr6 - 1 );
    p->force += 24*p->pair_lj.epsilon[ii][jj] * dr6/p->dr * (2*dr6/p->dr - 1);
}

/*
static void pair_LJ_short(t_pair *p){
    double dr6;
    dr6 = p->dr*p->dr*p->dr*p->dr*p->dr*p->dr;

    p->force = 24/p->dr/dr6*(2/dr6 - 1);
	p->epair = 4/dr6*(1/dr6 - 1); // Calculating potential energy.
}*/


/** \brief Run Leonard-Jones cut-off pair interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Potential energy and Force from Leonard-Jones cut-off potential
 */
static void pair_LJ_cutoff(t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;

    if (p->dr < p->pair_lj.r_cutoff_lj[ii][jj]){
        pair_LJ(p);
        p->epair += p->pair_lj.e_cutoff_lj[ii][jj];
    }
    else{
        p->force = 0;
        p->epair = 0;
    }
}


/** \brief Run Morse pair interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Potential energy and Force from Morse potential
 */
static void pair_Morse(t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;
    double var = exp(-p->pair_morse.beta[ii][jj]*(p->dr - p->pair_morse.r0[ii][jj]));
    p->force += 2*p->pair_morse.beta[ii][jj] * p->pair_morse.d[ii][jj] * var * (var - 1);
    p->epair += p->pair_morse.d[ii][jj] * var * (var - 2);
}


/** \brief Run Morse cut-off pair interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Potential energy and Force from Morse cut-off potential
 */
static void pair_Morse_cutoff(t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;
    if (p->dr <= p->pair_morse.r_morse_cutoff[ii][jj]){
        pair_Morse(p);
        p->epair += p->pair_morse.e_morse_cutoff[ii][jj];
    }
    else{
        p->force += 0.0;
        p->epair += 0.0;
    }
}


/** \brief Run Coulomb pair interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Potential energy and Force from Coulomb potential
 */
static void pair_coul(t_pSys *pSys, t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;
    p->pair_coul.eCoulumb  =   KCOUL*pSys->q[ii]*pSys->q[jj]/p->dr;
    p->force    +=   KCOUL*pSys->q[ii]*pSys->q[jj]/p->dr;
}


/** \brief Run Coulomb cut-off pair interaction
 *
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate Potential energy and Force from Coulomb cut-off potential
 */
static void pair_coul_cut(t_pSys *pSys, t_pair *p){
    int ii, jj;
    ii = p->ii; jj = p->jj;
    if (p->dr <= p->pair_coul.r_cutoff_coul){
        pair_coul(pSys, p);
        p->pair_coul.eCoulumb += KCOUL*pSys->q[ii]*pSys->q[jj]/p->pair_coul.r_cutoff_coul;
    }
    else{
        p->force    += 0.0;
        p->pair_coul.eCoulumb += 0.0;
    }
}

