/**
   @file      advance.h
   @brief     Advance module header: run advance style modeling.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef ADVANCE_H_INCLUDED
#define ADVANCE_H_INCLUDED

#include "struct.h"
#include "compute/compute.h"
#include "out/out.h"
#include "fix/fix.h"
#include "pair/pair.h"
#include "init.h"

/** \brief Advance style modeling
 *
 * \param  filename Name of configuration ini file
 * \return void
 *
 */
void advance (const char* filename);

#endif // ADVANCE_H_INCLUDED
