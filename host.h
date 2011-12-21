#ifndef HOST_H
#define HOST_H

#include <QThread>
#include <QTableWidget>
#include <QHostInfo>
#include <iostream>

class Host : public QThread
{
    Q_OBJECT
public:
    Host(QTableWidget *tab,int r,QString adr);
    void run();
private:
    QTableWidget *table;
    int row;
    QString adres;

private slots:
    void printResults(QHostInfo info);
};

#endif // HOST_H
