/**
   @file      fix.c
   @brief     Fix module.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "fix.h"


/** \brief Initialize the structure t_fix
 *
 * \param ini Dictionary to search
 * \return Initialized t_fix structure
 *
 */
t_fix fix_init(dictionary* ini){
    t_fix fix;
    fix.term = t_term_init(ini);
    return fix;
}


void fix_run (t_key *key, t_pSys *pSys, t_opts *opts, t_fix *fix){
    struct t_term term = fix->term;
    if (key->term_berendsen == 1) fix->term = term_berendsen(pSys, opts, &term);
    if (key->term_rescale   == 1) fix->term = term_rescale  (pSys, opts, &term);
}

