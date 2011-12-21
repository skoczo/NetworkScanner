/*
 * MessageFlow.h
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */

#ifndef MESSAGEFLOW_H_
#define MESSAGEFLOW_H_

#include <QObject>
#include <QQueue>
#include <QThread>
#include "MessageObject.h"
#include "mainwindow.h"

class MessageFlow : public QThread
{
	Q_OBJECT
private:
	static MessageFlow *instance;
	QQueue<MessageObject*> *queue;
	QObject* main;
	MessageFlow();

public:
	void sendMessage(MessageObject*);
	void registerWindow(QObject *);
	void run();
	static MessageFlow* getInstance()
	{
		if(!instance)
		{
			instance = new MessageFlow();
		}

		return instance;
	}
signals:
	    void message(MessageObject*);

};
#endif /* MESSAGEFLOW_H_ */
