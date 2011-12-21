#ifndef PING_H
#define PING_H

#include <QThread>
#include <QObject>
#include <QWidget>
#include <QString>
#include <QHostInfo>
#include <cstdlib>
#include <iostream>

class Ping : public QThread
{
    Q_OBJECT
private:
    int delay;
    QString adres;
    bool petla;

public:
    Ping(int d,QString a);
    ~Ping();
protected:
    void run();

signals:
    void dane(QString,int);
};

#endif // PING_H
