#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent), ui(new Ui::MainWindow)
{
	message = NULL;
	c = NULL;
	s = NULL;
	ui->setupUi(this);
	QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
	tray = new QSystemTrayIcon(QIcon(":/new/logo.png"));
	tray->show();
	is_visible = true;
	connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this,
			SLOT(show_hide(QSystemTrayIcon::ActivationReason)));
	connect(ui->zakoncz, SIGNAL(clicked()), this, SLOT(stop()));
	connect(MessageFlow::getInstance(), SIGNAL(message(MessageObject*)), this,
			SLOT(showInfo(MessageObject*)));
	setWindowTitle("Monitor sieci");

	MessageFlow::getInstance()->start();

	ui->tableWidget->setColumnCount(2);
	ui->tableWidget->setRowCount(0);
	ui->tableWidget->setHorizontalHeaderLabels(
			QStringList() << "Nazwa hosta" << "Adres");
	ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
}

MainWindow::~MainWindow()
{
	delete ui;
	stop();
}

void MainWindow::show_hide(QSystemTrayIcon::ActivationReason dana)
{
	if (dana == QSystemTrayIcon::DoubleClick)
	{
		if (is_visible)
		{
			hide();
			is_visible = false;
		}
		else
		{
			show();
			is_visible = true;
		}
	}
}

void MainWindow::ustaw(QString adres, int i)
{
	int ob = adresy_obecnosc[adres];
	if (ob != i)
	{
		if (i == 0)
		{
			QHostInfo info = QHostInfo::fromName(adres);
			tray->showMessage("",
					"Podłączono do " + adres + " " + info.hostName(),
					QSystemTrayIcon::NoIcon, 2000);
			adresy_obecnosc[adres] = i;
			ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
			int row = ui->tableWidget->rowCount() - 1;
			hosty[adres] = new Host(ui->tableWidget, row, adres);
			//std::cout<<"ROW: "<<row<<std::endl;
			ui->tableWidget->setItem(row, 0,
					new QTableWidgetItem(info.hostName()));
			ui->tableWidget->setItem(row, 1,
					new QTableWidgetItem(tr(adres.toLatin1().data())));

			/*startowanie watku sprawdzającego hosta*/
			hosty[adres]->start();
		}
		else
		{
			tray->showMessage("", "Odłączono " + adres,
					QSystemTrayIcon::Information, 2000);
			adresy_obecnosc[adres] = i;
			QList<QTableWidgetItem *> lista = ui->tableWidget->findItems(adres,
					Qt::MatchContains);

			if (lista.size() > 0) {
				int row = ui->tableWidget->row(lista[0]);
				ui->tableWidget->removeRow(row);

				/*konczenie watku sprawdzającego hosta*/
				hosty[adres]->quit();
			}
		}
	}
	else if (i == 0)
	{
		QList<QTableWidgetItem *> lista = ui->tableWidget->findItems(adres,
				Qt::MatchContains);

		qDebug()<<"lista->size(): "<<QString::number(lista.size())<<"\n";

		if (lista.size() > 0) {
			int row = ui->tableWidget->row(lista[0]);
			QTableWidgetItem *item = ui->tableWidget->item(row, 0);
			std::string str = item->text().toStdString();
			qDebug()<<"Tekst: "<<item->text()<<"\nAdres: "<<adres<<"\n";

			if (item->text() == adres) {
				QHostInfo info = QHostInfo::fromName(adres);
				qDebug()<<"Host name: "<<info.hostName()<<"\n";
				if (info.hostName() != adres) {
					item->setText(info.hostName());
				}
			}
		}
	}
}

void MainWindow::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void MainWindow::on_pushButton_clicked()
{
	closeAllThreads(false);

	if (s != NULL)
	{
		s->terminate();

		while (!s->isFinished())
		{
		}

		delete s;
	}

	Ping::setStarted(true);
	MessageFlow *message = MessageFlow::getInstance();
	message->sendMessage(new MessageObject(0, "Startowanie wątków", true));

	s = new Start(&list, &adresy_obecnosc, ui->tableWidget,
			ui->lineEdit->text(), ui->lineEdit_2->text(), this);
	s->start();

	message->sendMessage(new MessageObject(0, "Wątki wystartowane", false));
}

void MainWindow::closeAllThreads(bool notify)
{
	if (c != NULL)
		{
			if (!c->isFinished())
			{
				c->terminate();

				for (int retries = 0; retries < 100 && !c->isFinished(); retries++)
				{
					usleep(rand() % 100);
				}

				if (!c->isRunning())
					qDebug() << "thread is stopped";
				else
					qDebug() << "thread is not stopped";
			}
		}
		c = new Clean(&list, notify);
		c->start();
}

void MainWindow::stop()
{
	closeAllThreads(true);
}

void MainWindow::showInfo(MessageObject* m) {
	if (m->getIcon() == 0) {
		if (message != NULL) {
			message->close();
			delete (message);
		}
		message = new QMessageBox(QMessageBox::Information, "Informacja",
				m->getMessage(), QMessageBox::Ok, this, Qt::Dialog);
		message->setWindowModality(Qt::ApplicationModal);
		message->show();
		blockButtons(m->isBlocking());
	}
}

void MainWindow::blockButtons(bool block)
{
	ui->pushButton->setEnabled(!block);
	ui->zakoncz->setEnabled(!block);
}

Clean::Clean(QList<Ping*> *l, bool isNotify)
{
	list = l;
	this->isNotify = isNotify;
}

void Clean::run()
{
	if(isNotify)
	MessageFlow::getInstance()->sendMessage(
			new MessageObject(0, "Kończenie pracy wątków", true));

	Ping::setStarted(false);

	while (!Mutex::tryLock())
	{
	}

	for (int i = 0; i < list->size(); i++)
	{
		Ping *p = list->at(i);
		qDebug() << "try to stop thread";
		p->terminate();
		srand( time(NULL));
		for (int retries = 0; retries < 100 && !p->isFinished(); retries++)
		{
			usleep(rand() % 100);
		}

		if (!p->isRunning())
			qDebug() << "thread is stopped";
		else
			qDebug() << "thread is not stopped";

		//delete(p);
	}

	qDebug()<<"#################";
	for (int i = 0; i < list->size(); i++)
	{
		qDebug() << i << " "<< list->at(i)->isFinished();
	}
	qDebug()<<"#################";

	list->clear();

	qDebug()<<"list->size() "<< list->size();
	Mutex::unlock();

	if(isNotify)
	MessageFlow::getInstance()->sendMessage(
			new MessageObject(0, "Zamykanie wątków udane", false));
}

Start::Start(QList<Ping*> *list, QMap<QString, int> *adresy_obecnosc,
		QTableWidget *table, QString poczatek, QString koniec,
		MainWindow *mainW)
{
	this->list = list;
	this->adresy_obecnosc = adresy_obecnosc;
	this->poczatek = poczatek;
	this->koniec = koniec;
	this->table = table;
	this->mainW = mainW;
}

void Start::startThread()
{
	start();
}

void Start::run()
{
	while (!Mutex::tryLock())
	{
	}

	for (int i = table->rowCount() - 1; i >= 0; --i)
	{
		table->removeRow(i);
	}
	Ping::setStarted(true);

	QString tmp;
	int kropki = 0;
	for (int i = 0; i < poczatek.length(); i++)
	{
		if (kropki == 3)
		{
			tmp += poczatek[i];
		}

		if (poczatek[i] == '.')
			kropki++;
	}
	int start = tmp.toInt();
	std::cout << start << std::endl;

	tmp = "";
	kropki = 0;
	for (int i = 0; i < koniec.length(); i++)
	{
		if (kropki == 3)
		{
			tmp += koniec[i];
		}

		if (koniec[i] == '.')
			kropki++;
	}
	int stop = tmp.toInt();
	std::cout << stop << std::endl;

	if (stop - start <= 0)
	{
		MessageFlow::getInstance()->sendMessage(
				new MessageObject(0, "Błąd zakresu", false));
	}
	else
	{
		QString podstawa;
		kropki = 0;
		for (int i = 0; kropki < 3; i++)
		{
			if (poczatek[i] == '.')
				kropki++;
			podstawa += poczatek[i];
		}
		std::cout << podstawa.toStdString() << std::endl;
		std::cout << "List.size:" << list->size() << std::endl;

		for (int i = 0; i < stop - start; i++) {
			Ping *p = new Ping(5, podstawa + QString::number((start + i)));
			list->push_back(p);
			connect(p, SIGNAL(dane(QString,int)), mainW,
					SLOT(ustaw(QString,int)));
			msleep(100);
			p->start();
			(*adresy_obecnosc)[podstawa + QString::number((start + i))] = 256;
		}
	}

	Mutex::unlock();
}
