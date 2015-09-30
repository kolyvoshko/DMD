/**
   @file      leapfrog.h
   @brief     Leap frog module header: run leap frog style modeling.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 9 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef LEAPFROG_H_INCLUDED
#define LEAPFROG_H_INCLUDED

#include "struct.h"
#include "compute/compute.h"
#include "out/out.h"
#include "fix/fix.h"
#include "pair/pair.h"
#include "init.h"

/** \brief Leap frog style modeling
 *
 * \param  filename Name of configuration ini file
 * \return void
 *
 */
void leaprog(const char* filename);

#endif // LEAPFROG_H_INCLUDED
