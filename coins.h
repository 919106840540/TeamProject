#ifndef COINS_H
#define COINS_H

#include <QGraphicsTextItem>
class Coins : public QGraphicsTextItem
{
public:
    Coins(QGraphicsItem * parent = 0);
    void increase();
    int getCoins();
    int coins;
private:

};
#endif // COINS_H
