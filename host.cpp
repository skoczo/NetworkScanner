#include "host.h"
#include <Qt/qhostinfo.h>
#include <Qt/qdebug.h>
#include <Qt/qstring.h>

Host::Host(QTableWidget *tab, int r, QString adr)
{
    table=tab;
    row=r;
    adres=adr;
}

void Host::run()
{
    QHostInfo::lookupHost(adres,
                          this, SLOT(printResults(QHostInfo)));
}

void Host::printResults(QHostInfo info)
{
    std::cout<<"pobieram nazwe dla "<<adres.toStdString()<<std::endl;
    table->setItem(row,0,new QTableWidgetItem(info.hostName()));
}
