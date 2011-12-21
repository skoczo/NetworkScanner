/*
 * Mutex.cpp
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */

#include "Mutex.h"

QMutex* Mutex::m = new QMutex();
bool Mutex::isL = false;

Mutex::Mutex()
{
	if (m == NULL)
	{
		m = new QMutex();
	}

	if (m->tryLock())
	{
		isL = true;
	}
}
