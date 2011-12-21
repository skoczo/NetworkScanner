#include "ping.h"

Ping::Ping(int d, QString a)
{
    if(d<=0)
        delay=1;
    else
        delay=d;
    adres=a;
    petla=true;
}

Ping::~Ping()
{
    qDebug()<<"delete ping object";
}

void Ping::run()
{
    int wynik;
    srand(time(NULL));
    int r=rand()%9000;
    usleep(r);
   // std::cout<<r<<std::endl;
    while(petla)
    {
        wynik=system(("ping "+adres+" -c 1 -q -W 1").toLatin1().data());
        std::cout<<"wynik: "<<wynik<<std::endl;
        emit dane(adres,wynik);
        sleep(rand()%delay+5);
    }
}
