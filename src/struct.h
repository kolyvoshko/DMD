/**
   @file    struct.h
   @brief   Implements a system, options and key structures.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef STRUCT_H
#define STRUCT_H

#include "input/iniparser/iniparser.h"
/**
    \brief  System object

    This object saves variable system and its settings.
*/
typedef struct {
    int     dim;  /**< System dimension */
    int     nType; /**< Number of atom types */

    double   *x, /**< Coordinates array x. */
             *y, /**< Coordinates array y. */
             *z, /**< Coordinates array z. */
            *vx, /**< Velocities array x. */
            *vy, /**< Velocities array y. */
            *vz, /**< Velocities array z. */
            *ax, /**< Accelerations array x. */
            *ay, /**< Accelerations array y  */
            *az, /**< Accelerations array z. */
            *fx, /**< Forces array x */
            *fy, /**< Forces array y */
            *fz; /**< Forces array z */

    double  *mass, /**< Mass array */
            *q, /**< Charge array */
            *atomType; /**< Type of atom */

    double  lx, /**< System's linear sizes X in angstrom. */
            ly, /**< System's linear sizes Y in angstrom. */
            lz; /**< System's linear sizes Z in angstrom. */
    int     pCnt; /**< Particle count. */                  //
    double  eKin,/**< Kinetic energy */
            ePot;/**< Potential energy */
    double  temperature;/**< Temperature system */

    int     *type; /**< index of type */
    char    **nameType; /**< Name of type */
    double  *massType,/**< mass of type */
            *qType;/**< change of type */
} t_pSys;


/**
    \brief  Properties modeling module

    This object saves parameters modeling
*/
typedef struct {
    double  dt, /**<  Time step */
            elapTime, /**< Elapsed time */
            targTime; /**< Target time */
    int     thisIter, /**< No. of current iteration */
            targIters; /**< No. of target iteration */

    int     snapInterv; /**< Iterational interval of snapshots. */
    double  vmax; /**< Max velocity in Maxvel distribution */
}  t_opts;


/**
    \brief  Object saves keys. Keys enables or disables modules
    of the program.

    Key structures: program management modules
*/
typedef struct {
    /// COMPUTE
    int compute;        /**< Compute module key */
    int compute_dr;     /**< Compute radial function */
    int compute_rdf;    /**< Compute radial distribution function */
    int compute_temp;   /**< Compute temperature */

    /// PAIR
    int pair;           /**< Pair module key. Compute pair interaction */
    int pair_morse;     /**< Compute Morse interaction */
    int pair_morse_cut; /**< Compute Morse interaction. Cut-off function added */
    int pair_lj;        /**< Compute Leonard-Jones interaction */
    int pair_lj_cut;    /**< Compute Leonard-Jones interaction. Cut-off function added */
    int pair_coulumb;    /**< Compute Coulomb interaction */

    /// FIX
    int fix;            /**< fix module key */
    int term;           /**< Thermostat module key */
    int term_berendsen; /**< Thermostat Berendsen */
    int term_rescale;   /**< Thermostat rescale module key  */

    /// DUMP
    int dump;           /**< Dump module key */
    int dump_file;      /**< Dump data file module key */
    int dump_image;     /**< Dump image module key */
    int dump_movie;     /**< Dump movie module key */
    int print;           /**< print to console */

    /// DEBUG
    int debug;
    int log;
} t_key;


/** \brief Initialize the structure t_pSys
 *
 * \param ini Dictionary to search
 * \return Initialized t_Psys structure
 *
 */
t_pSys pSys_init (dictionary* ini);


/** \brief Initialize the structure t_opts
 *
 * \param ini Dictionary to search
 * \return Initialized t_opts structure
 *
 */
t_opts opts_init (dictionary* ini);


/** \brief Initialize the structure t_key
 *
 * \param ini Dictionary to search
 * \return Initialized t_key structure
 *
 */
t_key  key_init  (dictionary* ini);
#endif //STRUCT_H
