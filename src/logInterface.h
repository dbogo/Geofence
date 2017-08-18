
/*
LOG4C_PRIORITY_FATAL 	fatal
LOG4C_PRIORITY_ALERT 	alert
LOG4C_PRIORITY_CRIT 	crit
LOG4C_PRIORITY_ERROR 	error
LOG4C_PRIORITY_WARN 	warn
LOG4C_PRIORITY_NOTICE 	notice
LOG4C_PRIORITY_INFO 	info
LOG4C_PRIORITY_DEBUG 	debug
LOG4C_PRIORITY_TRACE 	trace
LOG4C_PRIORITY_NOTSET 	notset
LOG4C_PRIORITY_UNKNOWN 	unknown
*/

#ifndef LOGINTERFACE_H
#define LOGINTERFACE_H

#include <libs/log4c/include/log4c.h>

#define OPERATION_LOG_FILE "logs/operation.log"
#define NMEA_LOG_FILE "logs/nmea.log"
#define ERROR_LOG_FILE "logs/errors.log"

#define INFO 0
#define ERROR 1
#define NMEA 2

/**
 * @brief Represents a single type of logger (e.g. one for error, one for info, etc.)
 */
typedef struct logger{
	FILE* file;
	log4c_category_t* obj;
	char* instanceName;
} logger_t;


/**
 * @brief Holds all the loggers that are used to record different types of logs.
 */
typedef struct log_master{
	logger_t operationLogger;
	logger_t errLogger;
	logger_t nmeaLogger;
} log_master_t;

log_master_t logMaster;

/**
 * @brief      initializes the Loggers from log4c library
 * @param      logMaster  LogMaster struct that contains all loggers
 * @return	   0 for failure and 1 for success
 */
int initLogSystem(log_master_t *logMaster);

/**
 * @brief      Logs a message with INFO prioruty
 *
 * @param[in]  msg   the messages to log
 */
void log_info(const char *msg);

/**
 * @brief      Logs a message with ERROR priority
 *
 * @param[in]  msg   the message to log
 */
void log_err(const char *msg);

/**
 * @brief      close and stop the logging system
 * @return     errCode
 */
int finiLogSystem(void);

#endif /* LOGINTERFACE_H */
