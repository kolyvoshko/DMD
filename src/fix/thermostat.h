/**
   @file      thermostat.h
   @brief     Thermostat module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef THERMOSTAT_H_INCLUDED
#define THERMOSTAT_H_INCLUDED

#include "../struct.h"
#include "../compute/compute.h"

/**
    \brief  Thermostat module object

    This object saves variable thermostat commands.
*/
typedef struct t_term {
    int nIter;
    double dtT, thisTerm;

    double window, fraction, tStart, tStop;
    double tau;
} t_term;


/** \brief Initialize the structure t_term
 *
 * \param ini Dictionary to search
 * \return Initialized t_term structure
 *
 */
t_term t_term_init(dictionary *ini);


/** \brief Implementation rescale thermostat
 *
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param term  This object saves variable thermostat commands.
 * \return Thermostat module object update
 *
 */
struct t_term term_rescale    (t_pSys *pSys, t_opts *opts, struct t_term *term);


/** \brief Implementation Berendsen thermostat
 *
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param term  This object saves variable thermostat commands.
 * \return Thermostat module object update
 *
 */
struct t_term term_berendsen  (t_pSys *pSys, t_opts *opts, struct t_term *term);

#endif // THERMOSTAT_H_INCLUDED
