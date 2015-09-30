/**
 * @file    fix.h
 * @brief   Fix module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef FIX_H_INCLUDED
#define FIX_H_INCLUDED

#include "../struct.h"
#include "thermostat.h"

/**
    \brief  Fix module object

    This object saves variable fix commands.
*/
typedef struct _t_fix {
    struct t_term term;
} t_fix;


/** \brief Initialize the structure t_fix
 *
 * \param ini Dictionary to search
 * \return Initialized t_fix structure
 *
 */
t_fix fix_init(dictionary* ini);


/** \brief Run fix modules
 *
 * \param key   Key structures: program management modules
 * \param pSys  System object
 * \param opts  Properties modeling module
 * \param fix   This object saves variable fix commands
 * \return void
 *
 */
void fix_run (t_key *key, t_pSys *pSys, t_opts *opts, t_fix *fix);

#endif // FIX_H_INCLUDED
