/**
   @file    compute.h
   @brief   Compute module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef COMPUTE
#define COMPUTE

#define KBOLTZ 8.6173e-5 /**< [eV/K] */
#define EKIN_CONST 1 /**< 2.656e-6 */

#include "../struct.h"
#include "../pair/pair.h"
#include "rdf.h"

/**
 *  \brief  Compute commands object
 *
 *  This object saves variable compute commands.
*/
typedef struct t_compute {
    struct t_rdf rdf;
} t_compute;


/** \brief Initialize the structure t_compute
 *
 * \param ini Dictionary to search
 * \param key Key structures: program management modules
 * \return Initialized t_compute structure
 *
 */
t_compute compute_init (dictionary *ini, t_key *key);


/** \brief Run compute modules
 *
 * \param key   Key structures: program management modules
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param compute   This object saves variable compute commands.
 * \return void
 *
 */
void compute_run  (t_key *key, t_pSys *pSys, t_opts *opts, t_compute *compute);


/** \brief Run dr submodule compute module
 *
 * \param key   Key structures: program management modules
 * \param compute   This object saves variable compute commands.
 * \param dr    Double distance between two atoms variable
 * \return void
 *
 */
void compute_dr   (t_key *key, t_compute *compute, double *dr);


/** \brief Calculate system temperature
 *
 * \param pSys  System object
 * \return void
 *
 */
void computeTemp (t_pSys *pSys);
void computePairForces(t_key *key, t_pSys *pSys, t_pair *p);
#endif
