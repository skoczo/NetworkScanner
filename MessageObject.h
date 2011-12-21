/*
 * MessageObject.h
 *
 *  Created on: 17-12-2011
 *      Author: skoczeq
 */

#include <QMessageBox>
#include <QString>

#ifndef MESSAGEOBJECT_H_
#define MESSAGEOBJECT_H_


class MessageObject
{
private:
	int icon;
	QString message;
	bool block;

public:
	MessageObject(int, QString, bool);
	int getIcon(){return icon;}
	QString getMessage(){return message;}
	bool isBlocking(){return block;}
};


#endif /* MESSAGEOBJECT_H_ */
