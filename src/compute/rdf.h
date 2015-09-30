/**
   @file      rdf.h
   @brief     Compute radial distribution function module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef RDF
#define RDF
#include "../struct.h"

/**
    \brief  Compute radial distribution function commands object

    This object saves variable compute radial distribution function commands.
*/
typedef struct t_rdf {
    int    nCol, nStep, nCount, nStart, stepCounter;
    double rMax, dr;

    double *rdfArr;
    int size;
    char *output_file;
} t_rdf;


t_rdf init_RDF(dictionary *ini);
t_rdf writeRDFarray(t_rdf *rdf, double *dr);
void dump_rdf(t_opts *opts, t_rdf *rdf);
#endif
