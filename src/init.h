/**
   @file      init.c
   @brief     Initialization module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef INIT_H_INCLUDED
#define INIT_H_INCLUDED

#include "struct.h"
#include "compute/compute.h"
#include "pair/pair.h"

/** \brief initialize function initiates the initial properties of the system
 *
 * \return void
 *
 */
void init (t_key *key,
           t_pSys *pSys,
           t_opts *opts,
           t_pair *p,
           t_compute *compute);

#endif // INIT_H_INCLUDED
