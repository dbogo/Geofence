#include "logInterface.h"


int initLogSystem(log_master_t *logMaster){
	if(log4c_init()){
		printf("log4c_init() failed.\n");
		return 0;
	} else {
		//Instantiate a log4c_category_t with name instanceName
		logMaster->operationLogger.file = fopen(OPERATION_LOG_FILE, "a");
		logMaster->operationLogger.instanceName = "operation";
		logMaster->operationLogger.obj = log4c_category_get(logMaster->operationLogger.instanceName);
		log4c_category_set_appender(logMaster->operationLogger.obj,
									log4c_appender_get(OPERATION_LOG_FILE));

		logMaster->errLogger.file = fopen(OPERATION_LOG_FILE, "a");
		logMaster->errLogger.instanceName = "operation";
		logMaster->errLogger.obj = log4c_category_get(logMaster->errLogger.instanceName);
		log4c_category_set_appender(logMaster->errLogger.obj,
									log4c_appender_get(OPERATION_LOG_FILE));

		logMaster->nmeaLogger.file = fopen(NMEA_LOG_FILE, "a");
		logMaster->nmeaLogger.instanceName = "nmealog";
		logMaster->nmeaLogger.obj = log4c_category_get(logMaster->nmeaLogger.instanceName);
		log4c_category_set_appender(logMaster->nmeaLogger.obj,
									log4c_appender_get(NMEA_LOG_FILE));
	}
	return 1;
}

void log_info(log_master_t *lm, const char *msg){
	log4c_category_info(lm->operationLogger.obj, "%s", msg);
}

void log_err(log_master_t *lm, const char *msg){
	log4c_category_error(lm->errLogger.obj, "%s", msg);
}

int finiLogSystem(void){
	fclose(logMaster.operationLogger.file);
	fclose(logMaster.errLogger.file);
	fclose(logMaster.nmeaLogger.file);
	return log4c_fini();
}
