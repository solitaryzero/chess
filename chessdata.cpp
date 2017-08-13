#include "chessdata.h"

chessData::chessData()
{
    x=0;
    y=0;
    type=0;
}

chessData::chessData(int nx, int ny, int ntype)
{
    x=nx;
    y=ny;
    type=ntype;
}
