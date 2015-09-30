/**
   @file      verlet.h
   @brief     Verlet module header: run verlet style modeling.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   0.0
   @copyright GNU Public License.
*/

#ifndef VERLET_H_INCLUDED
#define VERLET_H_INCLUDED

#include "struct.h"
#include "compute/compute.h"
#include "out/out.h"
#include "fix/fix.h"
#include "pair/pair.h"
#include "init.h"

/** \brief Verlet frog style modeling
 *
 * \param  filename Name of configuration ini file
 * \return void
 *
 */
void verlet(const char* filename);

#endif // VERLET_H_INCLUDED
