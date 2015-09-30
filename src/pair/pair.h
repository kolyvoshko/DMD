/**
   @file      pair.h
   @brief     Pair module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 8 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef PAIR
#define PAIR
//#define KCOUL  14.40e-19 /**< [eV*A/(+e**2)] */
#define KCOUL  0 /**< [eV*A/(+e**2)] */
#include "../struct.h"

/**
    \brief Save Morse potential parameters.
*/
struct PairMorse {
    double **beta;
    double **d;
    double **r0;

    double **e_morse_cutoff;
    double **r_morse_cutoff;
};


/**
    \brief Save Leonard-Jones potential parameters.
*/
struct PairLJ {
    double **epsilon;
    double **sigma;

    double **r_cutoff_lj;
    double **e_cutoff_lj;
};


/**
    \brief Save Coulomb potential parameters.
*/
struct PairCoul {
    double r_cutoff_coul, e_cutoff_coul, eCoulumb;
};


/**
    \brief  Pair object

    This object saves variable pair interaction.
*/
typedef struct _t_pair {
    int ii, jj, nType;
    double dr, force, epair, neighbor;

    struct PairMorse    pair_morse;
    struct PairLJ       pair_lj;
    struct PairCoul     pair_coul;
} t_pair;


/** \brief Initialize the structure t_pair
 *
 * \param ini Dictionary to search
 * \return Initialized t_pair structure
 *
 */
t_pair t_pair_init(dictionary* ini);


/** \brief Initialize pair interaction
 *
 * \param key   Key structures: program management modules
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 * Calculate pair interaction constant matrix
 */
void pair_init (t_key *k, t_pair *p);


/** \brief Run pair interaction
 *
 * \param key   Key structures: program management modules
 * \param pSys  System object
 * \param p  This object have a variable pair interaction.
 * \return void
 *
 */
void pair (t_key *key, t_pSys *pSys, t_pair *p);

#endif
