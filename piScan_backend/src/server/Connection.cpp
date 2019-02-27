/*
 * Connection.cpp
 *
 *  Created on: Feb 12, 2019
 *      Author: ezra
 */

#include "ServerManager.h"

Connection::ConnectionLevel ClientRequest::permissionMap[] = {
		[SYSTEM_FUNCTION] = Connection::ConnectionLevel::FULL_CONTROL,
		[SCANNER_FUNCTION] = Connection::ConnectionLevel::FULL_CONTROL,
		[DATABASE_RETRIEVE] = Connection::ConnectionLevel::VIEWER,
		[DATABASE_MODIFY] = Connection::ConnectionLevel::FULL_CONTROL,
		[CONFIG_RETRIEVE] = Connection::ConnectionLevel::VIEWER,
		[CONFIG_MODIFY] = Connection::ConnectionLevel::FULL_CONTROL,
};


