/**
   @file        rdf.c
   @brief       Compute radial distribution function module.
   @author      Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date        July 30 2015
   @version     1.0
   @copyright   GNU Public License.
*/

#include "rdf.h"
#include "../out/out.h"


/** \brief Initialize t_rdf structure
 *
 * \param pSys  Structure which will be allocated memory
 * \param size  Size rdf array
 * \return void
 *
 */
static void Construct_t_rdf(t_rdf *rdf, int size){
  rdf->size = size;
  rdf->rdfArr = (double*)malloc(sizeof(double)*size);
}

/*
static void Destruct_t_rdf(t_rdf *rdf){
  free(rdf->rdfArr);
  rdf->rdfArr = 0;
}*/


/** \brief Initialize the structure t_rdf
 *
 * \param ini Dictionary to search
 * \return Initialized t_rdf structure
 *
 */
t_rdf init_RDF(dictionary *ini){
    double rMax, precision;

    precision = iniparser_getdouble(ini, "rdf:Precision", 0.1);
        if(precision < 0.0) ReportErrorMesage("ERROR: rdf:Precision must be positive double number");
    rMax      = iniparser_getdouble(ini, "rdf:Rmax", 10);
        if(rMax < 0.0) ReportErrorMesage("ERROR: rdf:Rmax must be positive double number");
    int size = rMax/precision;

    t_rdf rdf;
    Construct_t_rdf(&rdf, size);

    rdf.rMax     = rMax;
    rdf.size     = size;
    rdf.dr       = rMax/size;
    rdf.nCount   = 0;
    rdf.nStart   = iniparser_getint(ini, "rdf:Nstart", 10000);
        if(rdf.nStart < 1) ReportErrorMesage("ERROR: rdf:Nstart must be natural number");
    rdf.output_file = iniparser_getstring(ini, "rdf:Output", "rdf.dat");
    return rdf;
}


t_rdf writeRDFarray(t_rdf *rdf, double *dr){
    if( *dr < rdf->rMax && rdf->stepCounter > rdf->nStart ){
        int i;
        for(i=0; i < rdf->size; i++){
            if( *dr > i*rdf->dr && *dr < (i+1)*rdf->dr ) rdf->rdfArr[i]++;
        }
    }
    return *rdf;
}


void dump_rdf(t_opts *opts, t_rdf *rdf){
    int i;
    double sum = 0;
    for(i=0; i < rdf->size; i++) sum += rdf->rdfArr[i];

    if(sum!=0){
        for(i=0; i < rdf->size; i++) rdf->rdfArr[i]/=sum;
    }

    FILE *file;
    char fname[64];
    sprintf(fname, "output/rdf%06d.dat", opts->thisIter);

    if ((file = fopen(fname,"w")) == NULL)
        ReportErrorMesage("dump_rdf(): File not opened.\n");
    else{
        for(i = 0; i < rdf->size; i++)
           fprintf(file,"%f %f\n", i*rdf->dr, rdf->rdfArr[i]);
    }
    fclose(file);
}
