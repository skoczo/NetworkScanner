/*
 * MessageFlow.cpp
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */

#include "MessageFlow.h"

MessageFlow *MessageFlow::instance = 0;

MessageFlow::MessageFlow()
{
	queue = new QQueue<MessageObject*>();
}

void MessageFlow::registerWindow(QObject *o)
{
	main = o;
}

void MessageFlow::sendMessage(MessageObject* m)
{
	queue->enqueue(m);
}

void MessageFlow::run()
{
	while(true)
	{
		if(queue->size() == 0)
		{
			msleep(100);
			continue;
		}

		emit message(queue->dequeue());

	}
}
