/**
   @file      dump.h
   @brief     Dump module header.
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      July 30 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef DUMP
#define DUMP

#include "../struct.h"

/**
    \brief  Dump file properties

    This object saves properties dump image.
*/
struct DumpFile {
    int   snapInterv; /**< Interval snapshot */
    char* name; /**< Dump filename */
};

/**
    \brief  Dump image properties

    This object saves properties dump image.
*/
struct Image {
    int   snapInterv; /**< Interval snapshot */
    char* name; /**< Name of image */
};

/**
    \brief  Dump movie properties

    This object saves properties dump video.
*/
struct Movie {
    int   snapInterv; /**< Interval snapshot */
    char* name;/**< Name of movie */
};

/**
    \brief  Default out to console

    This object saves properties default out.
*/
struct Out {
    int snapInterv; /**< Interval snapshot */

    int kIter; /**< Key step output */
    int kPe; /**< Key potential energy output */
    int kKe; /**< Key kinetic output */
    int kE; /**< Key summary energy output */
    int kTemp; /**< Key temperature output */
};


/**
    \brief  Dump object

    This object saves variable dump commands.
*/
typedef struct  {
    struct DumpFile file;
    struct Image    img;
    struct Movie    mov;
    struct Out      out;
} t_dump;


/** \brief Initialize the structure t_dump
 *
 * \param ini Dictionary to search
 * \return Initialized t_dump structure
 *
 */
t_dump dump_init (dictionary *ini);


/** \brief Run dump output
 *
 * \param key Key structures: program management modules
 * \param pSys System structures
 * \param opts System options
 * \param dump Options dump output
 * \return void
 *
 */
void dump_run  (t_key *key, t_pSys *pSys, t_opts *opts, t_dump *dump);
void write_dump(t_pSys *pSys, t_opts *opts);
void writeTXTfile(char *name, double *dataArray, int nCount, int thisIter);
#endif
