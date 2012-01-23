#ifndef PING_H
#define PING_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QHostInfo>
#include <QMutex>
#include <cstdlib>
#include <iostream>

class Ping : public QThread
{
    Q_OBJECT
private:
    static QMutex* m;
    static bool isS;
    int delay;
    QString adres;
    bool petla;

public:
    Ping(int d,QString a);

    static void setStarted(bool isStart)
    {
		bool tmp = m->tryLock();

		if (tmp) {
		isS = isStart;
		}
		m->unlock();
	}

    static bool isStarted()
    {
    	bool temp = false;
		bool tmp = m->tryLock();

		if (tmp) {
		temp = isS;
		}
		m->unlock();
    	return temp;
    }

    ~Ping();
protected:
    void run();

signals:
    void dane(QString,int);
};

#endif // PING_H
