#include "coins.h"
#include<QFont>
Coins::Coins(QGraphicsItem * parent):QGraphicsTextItem (parent)
{
    //初始化：分数为0
    coins = 0;
    setPlainText(QString("Coins: ") + QString::number(coins));

    setDefaultTextColor(Qt::blue);
    setFont(QFont("times", 25));
}

void Coins::increase()
{
    coins ++;
    setPlainText(QString("Coins: ") + QString::number(coins));
}


int Coins::getCoins()
{
    return coins;
}
