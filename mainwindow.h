#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMessageBox>
#include <QMainWindow>
#include <QTextCodec>
#include <QList>
#include <QListIterator>
#include <QMap>
#include <QMapIterator>
#include <QSystemTrayIcon>
#include <QtNetwork/QHostInfo>
#include <QtNetwork/QHostAddress>
#include <QStringListModel>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QHostInfo>
#include <QTableWidgetItem>
#include <QIcon>
#include <QVector>
#include "MessageObject.h"
#include "MessageFlow.h"
#include "ping.h"
#include "host.h"
#include "Mutex.h"

namespace Ui
{
class MainWindow;
}

class Clean: public QThread
{
private:
	QList<Ping*> list;
	bool isNotify;

public:
	Clean(QList<Ping*> l, bool isNotify);
	void run();
};

class MainWindow;

class Start: public QThread
{
	Q_OBJECT
private:
	QList<Ping*> *list;
	QMap<QString, int> *adresy_obecnosc;
	QTableWidget *table;
	QString poczatek;
	QString koniec;
	MainWindow *mainW;

public:
	Start(QList<Ping*> *list, QMap<QString, int> *adresy_obecnosc,
			QTableWidget *table, QString poczatek, QString koniec, MainWindow *mainW);
	void run();
};

class MainWindow: public QMainWindow
{
Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	QList<Ping*> getList()
	{
		return list;
	}
	QMap<QString, int> getAdresyObecnosc()
	{
		return adresy_obecnosc;
	}
	~MainWindow();

protected:
	void changeEvent(QEvent *e);

private:
	Ui::MainWindow *ui;
	Ping *p;
	QList<Ping*> list;
	QMap<QString, int> adresy_obecnosc;
	QMap<QString, Host*> hosty;
	QSystemTrayIcon *tray;
	Clean *c;
	Start *s;
	bool is_visible;
	QMessageBox *message;
	void closeAllThreads(bool notify);

public slots:
    void ustaw(QString adres, int i);
    void show_hide(QSystemTrayIcon::ActivationReason dana);
private slots:
	void on_pushButton_clicked();
	void stop();
	void showInfo(MessageObject*);
};
#endif // MAINWINDOW_H
