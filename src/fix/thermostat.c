/**
   @file      thermostat.c
   @brief     Thermostat module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "thermostat.h"
#include "../out/out.h"

/** \brief Initialize the structure t_term
 *
 * \param ini Dictionary to search
 * \return Initialized t_term structure
 *
 */
t_term t_term_init(dictionary *ini){
    t_term term;

    term.nIter  = iniparser_getint(ini, "Thermostat:Niter" , 10);
        if (term.nIter < 1) ReportErrorMesage("ERROR: Thermostat:Niter must be natural number");
    term.tStart = iniparser_getint(ini, "Thermostat:Tstart", 40);
        if (term.tStart < 0) ReportErrorMesage("ERROR: Thermostat:Tstart must be positive number");
    term.tStop  = iniparser_getint(ini, "Thermostat:Tstop" , 40);
        if (term.tStop < 0) ReportErrorMesage("ERROR: Thermostat:Tstop must be positive number");

    // [Berensden]
    term.tau = iniparser_getint(ini, "Berensden:Tau", 10);
        if (term.tau < 1) ReportErrorMesage("ERROR: Berensden:Tau must be natural number");

    // [Rescale]
    term.fraction  = iniparser_getint(ini, "Rescale:Fraction", 1);
        if (term.fraction < 0) ReportErrorMesage("ERROR: Rescale:Fraction must be positive number");
    term.window    = iniparser_getint(ini, "Rescale:Window", 2);
        if (term.window < 0) ReportErrorMesage("ERROR: Rescale:Window must be positive number");

    int targIters = iniparser_getint(ini, "Time:Run", 10000);
        if (targIters < 1) ReportErrorMesage("ERROR: Time:Run must be natural number");

    term.thisTerm = term.tStart;
    term.dtT = (term.tStop - term.tStart)/(targIters/term.nIter);

    return term;
}


/** \brief Implementation rescale thermostat
 *
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param term  This object saves variable thermostat commands.
 * \return Thermostat module object update
 *
 */
struct t_term term_rescale (t_pSys *pSys, t_opts *opts, struct t_term *term){
    if(!(opts->thisIter % term->nIter)){
        computeTemp(pSys);
        term->thisTerm += term->dtT;
        if( 2*abs(pSys->temperature - term->thisTerm) > term->window ){
            double lambdaVel = 0;
            if( (pSys->temperature - term->thisTerm)>= 0){
                lambdaVel = sqrt( (pSys->temperature - term->fraction)/pSys->temperature);
            }
            if( (pSys->temperature - term->thisTerm)< 0){
                lambdaVel = sqrt( (pSys->temperature + term->fraction)/pSys->temperature);
            }


            int i;
            for(i=0; i<pSys->pCnt; i++){
                pSys->vx[i] = lambdaVel * pSys->vx[i];
                pSys->vy[i] = lambdaVel * pSys->vy[i];
                pSys->vz[i] = lambdaVel * pSys->vz[i];
            }
        }
    }
    return *term;
}


/** \brief Implementation Berendsen thermostat
 *
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param term  This object saves variable thermostat commands.
 * \return Thermostat module object update
 *
 */
struct t_term term_berendsen(t_pSys *pSys, t_opts *opts, struct t_term *term){
    if(!(opts->thisIter % term->nIter)){
        computeTemp(pSys);
        term->thisTerm += term->dtT;
        double lambdaVel;

        lambdaVel = sqrt( 1 + (term->nIter/term->tau)*(term->thisTerm/pSys->temperature - 1) );
        int i;
        for(i=0; i<pSys->pCnt; i++){
            pSys->vx[i] = lambdaVel * pSys->vx[i];
            pSys->vy[i] = lambdaVel * pSys->vy[i];
            pSys->vz[i] = lambdaVel * pSys->vz[i];
        }
    }
    return *term;
}
