#ifndef SHOP_H
#define SHOP_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "bird.h"
#include "block.h"
#include"button.h"
#include<QTimer>
#include<QGraphicsTextItem>
#include"textboard.h"
#include"score.h"


class Shop: public QGraphicsView
{
public:
    Shop(QWidget * parent=0);
    void Support();//显示支持我们的信息
    void Continue();//隐藏，回到游戏界面

    //Scene保存item的信息
    QGraphicsScene * scene;
    //生命值，子弹，支持我们
    //商品名称，商品， 购买按钮
    QGraphicsTextItem *Title;
    TextBoard *Life;
    TextBoard *Bullet;
    TextBoard *SupportUs;
    QGraphicsTextItem *money;
    QGraphicsTextItem *n_bullet;
    QGraphicsTextItem *pay;
    QGraphicsTextItem *n_life;



    Button * back;

    Button * BuyLife;
    Button * BuyBullet;
    Button * b_SupportUs;


signals:

public slots:
};

#endif // SHOP_H


