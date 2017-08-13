#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QList>
#include <QPainter>
#include <QPalette>
#include <QDebug>
#include <QMouseEvent>
#include "hostwindow.h"
#include "guestwindow.h"
#include "string.h"
#include <memory.h>
#include "chessdata.h"

class chessBoard : public QWidget
{
    Q_OBJECT
public:
    explicit chessBoard(QWidget *parent = 0);
    int currentPlayer,expectedPlayer;
    bool available;

protected:
    void paintEvent(QPaintEvent *ev);
    void mousePressEvent(QMouseEvent *ev);

signals:
    void gridClicked(int,int);
    void sbWin(int);

public slots:
    void addChess(int x,int y);
    void resetBoard();
    void statUpdate(int x,int y);
    int isSbWin();
    void searchForBombs();

private:
    int line[20][20][8],space[20][20][8];
    int used[20][20];
    bool bomb[20][20];
    int lineGap,margin;
    int dx[8],dy[8];
    bool isValid(int x,int y);
    int howManyBombs();
};

#endif // CHESSBOARD_H
