#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QPalette>
#include <QColor>
#include <QMessageBox>
#include "chessboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void seeHostWindow();
    void seeGuestWindow();
    void initServer();
    void acceptConnection();
    void recvMessage();
    void sendMessage(int i, int j);
    void connectHost(QString add);
    void connectSuccess();
    void disConnect();
    void declareWinner(int t);
    void requireReset();

private:
    Ui::MainWindow *ui;
    QTcpServer *listenSocket;
    QTcpSocket *readWriteSocket;
    guestWindow *gw;
    hostWindow *hw;
    void labelUpdate();
};

#endif // MAINWINDOW_H
