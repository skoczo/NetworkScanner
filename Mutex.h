/*
 * Mutex.h
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <QMutex>

class Mutex
{
private:
	static QMutex* m;
	static bool isL;
public:
	Mutex();

	static bool tryLock()
	{
		if (m == NULL)
		{
			m = new QMutex();
		}

		bool tmp = m->tryLock();

		if (tmp)
		{
			isL = tmp;
		}
		return tmp;
	}

	static void unlock()
	{
		if (m == NULL)
		{
			m = new QMutex();
		}

		m->unlock();

		isL = false;
	}

	static bool isLocked()
	{
		return isL;
	}
};


#endif /* MUTEX_H_ */
