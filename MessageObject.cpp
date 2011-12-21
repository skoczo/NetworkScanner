/*
 * MessageObject.cpp
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */
#include "MessageObject.h"

MessageObject::MessageObject(int i, QString message, bool isBlocking)
{
	this->icon = i;
	this->message = message;
	this->block = isBlocking;
}
