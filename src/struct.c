/**
   @file      struct.c
   @brief     Implements a system, options and key structures.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#include "struct.h"
#include "out/out.h"
#include "utils/utils.h"

static void Construct_t_pSys (t_pSys *pSys, int pCnt, int nType);


/** \brief Initialize the structure t_pSys
 *
 * \param ini Dictionary to search
 * \return Initialized t_Psys structure
 *
 */
t_pSys pSys_init (dictionary* ini){
    // TODO for f
    #pragma GCC diagnostic ignored "-Wunused-but-set-variable"

    t_pSys pSys;

    char *f;
    char *k;

    // [System]
    pSys.dim   = iniparser_getint(ini, "System:Dimension", 3);
    if (pSys.dim < 1 || pSys.dim > 3)
    ReportErrorMesage("ERROR: System:Dimension must be natural number in the range of 1...3: src/struct.c pSys_init");

    // [Data]
    k = iniparser_getstring(ini, "Data:Mode", "Set");
    if (strcmp(k, "Set") == 0){
        pSys.nType = iniparser_getint(ini, "Data:Ntype",  2);
        if (pSys.nType < 1) ReportErrorMesage("ERROR: Ntype must be natural number: src/struct.c pSys_init");
        pSys.pCnt  = iniparser_getint(ini, "Data:Ncount", 100);
        if (pSys.pCnt < 1) ReportErrorMesage("ERROR: Ncount must be natural number: src/struct.c pSys_init");
    }
    else if (strcmp(k, "File") == 0){
        f = iniparser_getstring(ini, "Data:Input", "input/input/he.dat");
    }
    else{
        ReportErrorMesage("ERROR: Data:Mode Unknown mode type \"%s\": src/struct.c key_init", k);
    }

    Construct_t_pSys (&pSys, pSys.pCnt, pSys.nType);

    return pSys;
}


/** \brief Initialize the structure t_opts
 *
 * \param ini Dictionary to search
 * \return Initialize t_opts structure
 *
 */
t_opts opts_init (dictionary* ini){
    t_opts opts;
    opts.targIters = iniparser_getint(ini, "Time:Run", 10000);
    if (opts.targIters < 1) ReportErrorMesage("ERROR: runtime must be natural number: src/struct.c opts_init ");
    opts.dt = iniparser_getdouble(ini, "Time:TimeStep", 1e-4);
    if (opts.dt < 0.0) ReportErrorMesage("ERROR: time step < must be positive double number: src/struct.c opts_init");
    return opts;
}

/** \brief Initialize the structure t_key
 *
 * \param ini Dictionary to search
 * \return Initialize t_key structure
 *
 */
t_key  key_init (dictionary* ini){
    t_key key;
    char *k, *k2, *k3;

    // [General]
    k = iniparser_getstring(ini, "General:Compute", "ON");
    key.compute = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: General:Compute must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "General:Fix", "ON");
    key.fix = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: General:Fix must be \"ON\" or \"OFF\": src/struct.c key_init");

    // [Potential]
    k = iniparser_getstring(ini, "Potential:Included", "OFF");
    key.pair = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: Potential:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k2 = iniparser_getstring(ini, "Potential:Cut", "ON");
    if (!xor(strcmp(k2, "ON"), strcmp(k2, "OFF")))
    ReportErrorMesage("ERROR: Potential:Cut must be \"ON\" or \"OFF\": src/struct.c key_init");

    k3 = iniparser_getstring(ini, "Potential:Type", "MORSE");
    if (
        !xor(!xor(strcmp(k3, "MORSE"),
                  strcmp(k3, "LJ")),
             strcmp(k3, "COULUMB"))
        )
    ReportErrorMesage("ERROR: Potential:Type Unknown potential type \"%s\": src/struct.c key_init", k3);

    key.pair_morse_cut  = (strcmp(k3, "MORSE") == 0 && strcmp(k2, "ON")  == 0) ? 1 : 0;
    key.pair_morse      = (strcmp(k3, "MORSE") == 0 && strcmp(k2, "OFF") == 0) ? 1 : 0;
    key.pair_lj_cut     = (strcmp(k3, "LJ")    == 0 && strcmp(k2, "ON")  == 0) ? 1 : 0;
    key.pair_lj         = (strcmp(k3, "LJ")    == 0 && strcmp(k2, "OFF") == 0) ? 1 : 0;
    key.pair_coulumb    = (strcmp(k3, "COULUMB") == 0) ? 1 : 0;


    // [Thermostat]
    k = iniparser_getstring(ini, "Thermostat:Included", "OFF");
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: Thermostat:Included must be \"ON\" or \"OFF\": src/struct.c key_init");
    key.fix  = (strcmp(k, "ON") == 0) ? 1 : 0;
    key.term = (strcmp(k, "ON") == 0) ? 1 : 0;

    k2 = iniparser_getstring(ini, "Thermostat:Type", "BERENSDEN");
    if (!xor(strcmp(k2, "BERENSDEN"), strcmp(k2, "RESCALE")))
    ReportErrorMesage("ERROR: Thermostat:Type Unknown thermostat type \"%s\": src/struct.c key_init", k2);
    key.term_berendsen = (strcmp(k2, "BERENSDEN") == 0) ? 1 : 0;
    key.term_rescale   = (strcmp(k2, "RESCALE") == 0) ? 1 : 0;

    // [Dump]
    k = iniparser_getstring(ini, "Dump:Included", "OFF");
    key.dump = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: Dump:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "DumpFile:Included", "OFF");
    key.dump_file = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: DumpFile:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "DumpImage:Included", "OFF");
    key.dump_image = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: DumpImage:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "DumpMovie:Included", "OFF");
    key.dump_movie = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: DumpMovie:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "Print:Included", "ON");
    key.print      = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: Print:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    k = iniparser_getstring(ini, "rdf:Included", "OFF");
    key.compute_dr  = (strcmp(k, "ON") == 0) ? 1 : 0;
    key.compute_rdf = (strcmp(k, "ON") == 0) ? 1 : 0;
    if (!xor(strcmp(k, "ON"), strcmp(k, "OFF")))
    ReportErrorMesage("ERROR: rdf:Included must be \"ON\" or \"OFF\": src/struct.c key_init");

    return key;
}


/** \brief Initialize t_pSys structure
 *
 * \param pSys  Structure which will be allocated memory
 * \param size  Number of atoms
 * \param nType Number types of atoms
 * \return void
 *
 */
static void Construct_t_pSys (t_pSys *pSys, int size, int nType){

    pSys->x = (double*)malloc(sizeof(double)*size);
    pSys->y = (double*)malloc(sizeof(double)*size);
    pSys->z = (double*)malloc(sizeof(double)*size);

    pSys->vx = (double*)malloc(sizeof(double)*size);
    pSys->vy = (double*)malloc(sizeof(double)*size);
    pSys->vz = (double*)malloc(sizeof(double)*size);

    pSys->ax = (double*)malloc(sizeof(double)*size);
    pSys->ay = (double*)malloc(sizeof(double)*size);
    pSys->az = (double*)malloc(sizeof(double)*size);

    pSys->fx = (double*)malloc(sizeof(double)*size);
    pSys->fy = (double*)malloc(sizeof(double)*size);
    pSys->fz = (double*)malloc(sizeof(double)*size);

    pSys->mass     = (double*)malloc(sizeof(double)*size);
    pSys->q        = (double*)malloc(sizeof(double)*size);
    pSys->atomType = (double*)malloc(sizeof(double)*size);

    pSys->type     = (int*)malloc(sizeof(int)*nType);
    pSys->massType = (double*)malloc(sizeof(double)*nType);
    pSys->qType    = (double*)malloc(sizeof(double)*nType);

    pSys->nameType = (char**) calloc(nType, sizeof(char*));

    int i;
    for ( i = 0; i < nType; i++ )
        pSys->nameType[i] = (char*) malloc(sizeof(char)*12);

    pSys->eKin       = 0;
    pSys->ePot       = 0;
}

/*
static void Destruct_t_pSys(t_pSys *pSys){
    free(pSys->x);
    free(pSys->y);
    free(pSys->x);

    free(pSys->vx);
    free(pSys->vy);
    free(pSys->vx);

    free(pSys->ax);
    free(pSys->ay);
    free(pSys->ax);

    free(pSys->mass);
    free(pSys->q);
    free(pSys->atomType);

    pSys->vx = 0;
    pSys->vy = 0;
    pSys->vz = 0;

    pSys->ax = 0;
    pSys->ay = 0;
    pSys->az = 0;

    pSys->mass = 0;
    pSys->q = 0;
    pSys->atomType = 0;

    free(pSys->type);
    free(pSys->massType);
    free(pSys->qType);
    free(pSys->nameType);

    pSys->type = 0;
    pSys->massType = 0;
    pSys->qType = 0;
    pSys->nameType = 0;
}*/


