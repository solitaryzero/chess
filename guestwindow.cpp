#include "guestwindow.h"

guestWindow::guestWindow(QWidget *parent):QDialog(parent)
{
    outerLayout = new QVBoxLayout;
    showerLayout = new QHBoxLayout;
    numberLayout = new QGridLayout;
    decisionLayout = new QGridLayout;
    mapper = new QSignalMapper(this);
    this->setWindowTitle("Connecting");
    ipNumber = "";

    label = new QLabel("Enter IP: ");
    ipBoard = new QLineEdit("");
    ipBoard->setReadOnly(true);
    showerLayout->addWidget(label);
    showerLayout->addWidget(ipBoard);

    int i,j;
    for (i=0;i<=1;i++)
        for (j=0;j<=4;j++)
        {
            buttons[i][j] = new QPushButton(QString::number(i*5+j));
            connect(buttons[i][j],SIGNAL(clicked(bool)),mapper,SLOT(map()));
            mapper->setMapping(buttons[i][j],i*5+j);
            numberLayout->addWidget(buttons[i][j],i,j);
            buttons[i][j]->setFixedSize(20,20);
        }
    dotButton = new QPushButton(".");
    connect(dotButton,SIGNAL(clicked(bool)),mapper,SLOT(map()));
    mapper->setMapping(dotButton,10);
    dotButton->setFixedSize(20,20);
    numberLayout->addWidget(dotButton,0,5);

    resetButton = new QPushButton("Reset");
    delButton = new QPushButton("Del");
    okButton = new QPushButton("OK");
    cancelButton = new QPushButton("Cancel");
    decisionLayout->addWidget(resetButton,0,0);
    decisionLayout->addWidget(delButton,0,1);
    decisionLayout->addWidget(okButton,1,0);
    decisionLayout->addWidget(cancelButton,1,1);

    outerLayout->addLayout(showerLayout);
    outerLayout->addLayout(numberLayout);
    outerLayout->addLayout(decisionLayout);
    this->setLayout(outerLayout);

    connect(okButton,SIGNAL(clicked(bool)),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(resetButton,SIGNAL(clicked(bool)),this,SLOT(resetIP()));
    connect(delButton,SIGNAL(clicked(bool)),this,SLOT(deleteIP()));
    connect(mapper,SIGNAL(mapped(int)),this,SLOT(addNumber(int)));
}

void guestWindow::resetIP()
{
    ipBoard->setText("");
}

void guestWindow::deleteIP()
{
    int last = ipBoard->text().size()-1;
    if (last < 0) return;
    QString s = ipBoard->text();
    s.remove(last,1);
    ipBoard->setText(s);
}

void guestWindow::addNumber(int num)
{
    QString s = ipBoard->text();
    if (num<=9)
    {
        s.append(QString::number(num));
    }
    else
    {
        s.append(".");
    }
    ipBoard->setText(s);
}

void guestWindow::accept()
{
    ipNumber = ipBoard->text();
    QDialog::accept();
}
