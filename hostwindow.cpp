#include "hostwindow.h"

hostWindow::hostWindow(QWidget *parent):QDialog(parent)
{
    this->setWindowTitle("Waiting for Connection");
    label = new QLabel("Host IP: ");
    hostIP = new QLineEdit;
    hostIP->setReadOnly(true);
    hostIP->setText(ipSearch());
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    hostLayout = new QGridLayout();
    hostLayout->addWidget(label,0,0);
    hostLayout->addWidget(hostIP,0,1);
    hostLayout->addWidget(okButton,1,0);
    hostLayout->addWidget(cancelButton,1,1);
    this->setLayout(hostLayout);

    connect(this->okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(this->cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
}

QString hostWindow::ipSearch()
{
    qDebug()<<QHostInfo::localHostName();
    QString name = QHostInfo::localHostName();
    QHostInfo info = QHostInfo::fromName(name);
    QHostAddress addr;
    foreach(addr,info.addresses())
    {
         if(addr.protocol() == QAbstractSocket::IPv4Protocol)
         return(addr.toString());
    }
    return info.addresses()[0].toString();
}
