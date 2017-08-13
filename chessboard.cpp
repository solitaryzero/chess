#include "chessboard.h"

chessBoard::chessBoard(QWidget *parent) : QWidget(parent)
{
    currentPlayer = 1;
    expectedPlayer = 1;
    margin = 13;
    lineGap = 35;
    available = false;
    dx[0]=0; dx[1]=1; dx[2]=1; dx[3]=1;
    dx[4]=0; dx[5]=-1; dx[6]=-1; dx[7]=-1;
    dy[0]=1; dy[1]=1; dy[2]=0; dy[3]=-1;
    dy[4]=-1; dy[5]=-1; dy[6]=0; dy[7]=1;
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    memset(line,0,sizeof(line));
    memset(used,0,sizeof(used));
    memset(space,0,sizeof(space));
    memset(bomb,0,sizeof(bomb));
}

bool chessBoard::isValid(int x,int y)
{
    if ((x<1) || (x>15)) return true;
    if ((y<1) || (y>15)) return false;
    return true;
}

void chessBoard::statUpdate(int x,int y)
{
    if (used[x][y] == 0) return;
    int px,py,i;

    for (i=0;i<=7;i++)
    {
        px = x;
        py = y;
        line[x][y][i]=0;
        while ((isValid(px,py)) && (used[px][py] == used[x][y]))
        {
            line[x][y][i]++;
            px = px + dx[i];
            py = py + dy[i];
        }
    }

    for (i=0;i<=7;i++)
    {
        px = x;
        py = y;
        space[x][y][i]=0;
        while ((isValid(px,py)) && ((used[px][py] == used[x][y]) || (used[px][py] == 0)))
        {
            space[x][y][i]++;
            px = px + dx[i];
            py = py + dy[i];
        }
    }

}

int chessBoard::isSbWin()
{
    int i,j,k;
    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
            for (k=0;k<=3;k++)
            {
                if (used[i][j] == 0) continue;
                if (line[i][j][k]+line[i][j][k+4] == 6)
                    return used[i][j];
            }

    return 0;
}

void chessBoard::addChess(int x,int y)
{
    used[x][y] = currentPlayer;
    currentPlayer = -currentPlayer;

    int i,j;
    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
            statUpdate(i,j);

    int t = isSbWin();
    if (t != 0) emit sbWin(t);
    update();
}

void chessBoard::resetBoard()
{
    memset(line,0,sizeof(line));
    memset(used,0,sizeof(used));
    memset(space,0,sizeof(space));
    memset(bomb,0,sizeof(bomb));
    currentPlayer = 1;
    update();
}

void chessBoard::searchForBombs()
{
    memset(bomb,0,sizeof(bomb));
    int b = howManyBombs();
    int i,j;
    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
            if (used[i][j] == 0)
            {
                used[i][j] = -expectedPlayer;
                int i1,j1;
                for (i1=1;i1<=15;i1++)
                    for (j1=1;j1<=15;j1++)
                        statUpdate(i1,j1);
                int c = howManyBombs();
                if (c>b)
                {
                    bomb[i][j] = 1;
                    qDebug() << "here at " << i << " " << j;
                }
                used[i][j] = 0;
                for (i1=1;i1<=15;i1++)
                    for (j1=1;j1<=15;j1++)
                        statUpdate(i1,j1);
            }
    update();
}

int chessBoard::howManyBombs()
{
    int ans = 0;
    int i,j,k,san,si;
    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
            if (used[i][j] == -expectedPlayer)
            {
                san = 0; si = 0;
                for (k=0;k<=3;k++)
                {
                    if ((line[i][j][k]+line[i][j][k+4] == 4) && (space[i][j][k]-1>=line[i][j][k])
                            && (space[i][j][k+4]-1>=line[i][j][k+4]))
                    {
                        san++;
                    }
                    if ((line[i][j][k]+line[i][j][k+4] == 5) && (space[i][j][k]+space[i][j][k+4] > 5)
                            && ((space[i][j][k] == line[i][j][k]) || (space[i][j][k+4] == line[i][j][k+4])))
                    {
                        si++;
                    }
                }

                ans = ans + san*san + san*si;
            }

    return ans;
}

void chessBoard::paintEvent(QPaintEvent *ev)
{
    int i,j;

    QPalette pl;
    this->setAutoFillBackground(true);
    pl.setBrush(QPalette::Window,QBrush(QPixmap(":/pics/board.jpg")));
    this->setPalette(pl);

    QPainter *p = new QPainter(this);
    QPixmap chessPic;
    p->setPen(Qt::black);

    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
        {
            if (used[i][j] == 0)
            {
                continue;
            }
            if (used[i][j] == 1)
            {
                chessPic = QPixmap(":/pics/black.png");
            }
            if (used[i][j] == -1)
            {
                chessPic = QPixmap(":/pics/white.png");
            }
            chessPic.scaled(30,30);
            p->drawPixmap(lineGap*i-margin-15,lineGap*j-margin-15,30,30,chessPic);
        }

    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
        {
            if (bomb[i][j] == 1)
            {
                qDebug() << "bomb at " << i << " " << j;
                chessPic = QPixmap(":/pics/bomb.png");
                chessPic.scaled(30,30);
                p->drawPixmap(lineGap*i-margin-15,lineGap*j-margin-15,30,30,chessPic);
            }
        }
    memset(bomb,0,sizeof(bomb));

    delete(p);
}

void chessBoard::mousePressEvent(QMouseEvent *ev)
{
    if (!available) return;
    int i,j;
    for (i=1;i<=15;i++)
        for (j=1;j<=15;j++)
        {
            if ((abs(ev->x()-(lineGap*i-margin))<15) && (abs(ev->y()-(lineGap*j-margin))<15))
            {
                if ((used[i][j] != 0) || (currentPlayer != expectedPlayer)) return;
                addChess(i,j);
                emit gridClicked(i,j);
                return;
            }
        }
}
