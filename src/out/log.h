/**
   @file      log.h
   @brief     Log header file
   @author    Eugene Kolivoshko (ekolivoshko@gmail.com)
   @date      August 9 2015
   @version   1.0
   @copyright GNU Public License.
*/

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED


/** \brief logs a ERROR message; execution is interrupted
 *
 * \param  format ERROR message
 * \param  ... ERROR message variables
 * \return void
 */
void ReportErrorMesage     (const char *format, ...);


/** \brief logs a WARNING message; execution don't interrupted
 *
 * \param  format WARNING message
 * \param  ... WARNING message variables
 * \return void
 */
void ReportWarningMessage (const char *format, ...);


/** \brief logs a message; execution don't interrupted
 *
 * \param  format message
 * \param  ... message variables
 * \return void
 */
void ReportMessage (const char *format, ...);

#endif // LOG_H_INCLUDED
