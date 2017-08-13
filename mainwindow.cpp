#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setContentsMargins(0,0,0,0);
    this->ui->disconnectButton->setEnabled(false);
    this->ui->hintButton->setEnabled(false);
    this->ui->resetButton->setEnabled(false);
    connect(ui->hostButton,SIGNAL(clicked(bool)),this,SLOT(seeHostWindow()));
    connect(ui->guestButton,SIGNAL(clicked(bool)),this,SLOT(seeGuestWindow()));
    connect(ui->board,SIGNAL(gridClicked(int,int)),this,SLOT(sendMessage(int,int)));
    connect(ui->board,SIGNAL(sbWin(int)),this,SLOT(declareWinner(int)));
    connect(ui->disconnectButton,SIGNAL(clicked(bool)),this,SLOT(disConnect()));
    connect(ui->resetButton,SIGNAL(clicked(bool)),this,SLOT(requireReset()));
    connect(ui->hintButton,SIGNAL(clicked(bool)),ui->board,SLOT(searchForBombs()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::declareWinner(int t)
{
    QString Winner;
    if (t == 1)
    {
        Winner = "Black Wins!";
    }
    else
    {
        Winner = "White Wins!";
    }

    QMessageBox *box = new QMessageBox(QMessageBox::Information,"Game End",Winner,QMessageBox::Ok);
    box->show();
    ui->board->available = false;
}

void MainWindow::requireReset()
{
    sendMessage(0,1);
}

void MainWindow::seeHostWindow()
{
    hw = new hostWindow(this);
    if (hw->exec() == QDialog::Accepted)
    {
        initServer();
        return;
    }
    else
    {
        return;
    }
}

void MainWindow::seeGuestWindow()
{
    gw = new guestWindow(this);
    if (gw->exec() == QDialog::Accepted)
    {
        connectHost(gw->ipNumber);
    }
    else
    {
        return;
    }
}

void MainWindow::disConnect()
{
    if(listenSocket != nullptr) listenSocket->close();

    if (readWriteSocket != nullptr)
    {
        sendMessage(0,0);
        this->ui->label->setText("");
        this->ui->label2->setText("");
        this->ui->board->available = false;
        readWriteSocket->close();
    }
    this->ui->hostButton->setEnabled(true);
    this->ui->guestButton->setEnabled(true);
    this->ui->disconnectButton->setEnabled(false);
    this->ui->hintButton->setEnabled(false);
    this->ui->resetButton->setEnabled(false);
    this->ui->board->resetBoard();
}

void MainWindow::connectHost(QString add)
{
    this->readWriteSocket = new QTcpSocket;
    this->readWriteSocket->connectToHost(QHostAddress(add),8888);
    connect(this->readWriteSocket,SIGNAL(connected()),this,SLOT(connectSuccess()));
}

void MainWindow::connectSuccess()
{
    this->ui->hostButton->setEnabled(false);
    this->ui->guestButton->setEnabled(false);
    this->ui->disconnectButton->setEnabled(true);
    this->ui->hintButton->setEnabled(true);
    this->ui->resetButton->setEnabled(true);
    this->ui->board->expectedPlayer = -1;
    this->ui->board->available = true;
    this->ui->label2->setText("White");
    labelUpdate();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void MainWindow::initServer()
{
    this->listenSocket = new QTcpServer;
    this->listenSocket->listen(QHostAddress::Any,8888);
    this->ui->hostButton->setEnabled(false);
    this->ui->guestButton->setEnabled(false);
    this->ui->disconnectButton->setEnabled(true);
    QObject::connect(this->listenSocket,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void MainWindow::acceptConnection()
{
    this->readWriteSocket =this->listenSocket->nextPendingConnection();
    this->listenSocket->close();
    this->ui->board->expectedPlayer = 1;
    this->ui->board->available = true;
    this->ui->label2->setText("Black");
    this->ui->hintButton->setEnabled(true);
    this->ui->resetButton->setEnabled(true);
    labelUpdate();
    QObject::connect(this->readWriteSocket,SIGNAL(readyRead()),this,SLOT(recvMessage()));
}

void MainWindow::recvMessage()
{
    QString info;
    info += this->readWriteSocket->readAll();
    int i,j;
    i = info.section(" ",0,0).toInt();
    j = info.section(" ",1,1).toInt();
    if ((i==0) && (j==0))
    {
        disConnect();
        this->ui->label->setText("");
        return;
    }

    if ((i==0) && (j==1))
    {
        QMessageBox *box = new QMessageBox(QMessageBox::Question,"Reset","Your opponent wants to reset the game.",
                                           QMessageBox::Yes | QMessageBox::No);
        if (box->exec() == QMessageBox::Yes)
        {
            ui->board->resetBoard();
            labelUpdate();
            sendMessage(0,2);
        }
        return;
    }

    if ((i==0) && (j==2))
    {
        ui->board->resetBoard();
        labelUpdate();
        return;
    }

    this->ui->board->addChess(i,j);
    labelUpdate();
}

void MainWindow::sendMessage(int i,int j)
{
    QByteArray *array = new QByteArray;
    array->clear();
    QString t = QString::number(i)+" "+QString::number(j);
    array->append(t);
    this->readWriteSocket->write(array->data());
    labelUpdate();
}

void MainWindow::labelUpdate()
{
    int i = this->ui->board->currentPlayer;
    if (i == -1)
    {
        /*
        QPalette p = this->palette();
        p.setColor(QPalette::Text,Qt::white);
        this->ui->label->setPalette(p);
        */
        this->ui->label->setText("White");
    }
    else
    {
        /*
        QPalette p = this->palette();
        p.setColor(QPalette::Text,Qt::black);
        this->ui->label->setPalette(p);
        */
        this->ui->label->setText("Black");
    }
}
