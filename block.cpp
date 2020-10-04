#include "block.h"
#include <QTimer>
#include <QGraphicsScene>
#include <QList>
#include <stdlib.h>
#include "game.h"
#include <QBrush>
#include<QRadialGradient>
#include<QLinearGradient>

extern Game * game;

Block::Block(QGraphicsItem *parent): QObject(), QGraphicsRectItem(parent)
{
    this->width = 125;
    this->height = 350;
    this->setRect(0, 0, this->width, this->height);
    this->setPos(1000, 0);



//    日后渐变
//    QRadialGradient gradient();
//    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
//    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));

//    QBrush brush(gradient);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(QColor(50,205,50));
    setBrush(brush);


    timer = new QTimer(this);
    timer->start(5);
    connect(timer, SIGNAL(timeout()), SLOT(move()));
    //如果发生碰撞，block停止运动
    connect(game->birdy, &bird::gameover, this, &Block::stop);
    connect(game->birdy, &bird::GoOn, this, &Block::ReConnect);

}

void Block::move()
{
    this->setPos(this->x() - 1, this->y());
  //  如果超过了边界，释放空间，后期增加子弹功能小心多次释放
    if(this->x() < 0 - this->width)
            delete this;

}

void Block::stop()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

void Block::ReConnect()
{
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}



