#ifndef HOSTWINDOW_H
#define HOSTWINDOW_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QHostAddress>
#include <QHostInfo>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

class hostWindow : public QDialog
{
    Q_OBJECT

public:
    explicit hostWindow(QWidget *parent = 0);

private:
    QLabel *label;
    QLineEdit *hostIP;
    QPushButton *okButton,*cancelButton;
    QGridLayout *hostLayout;
    QString ipSearch();
};

#endif // HOSTWINDOW_H
