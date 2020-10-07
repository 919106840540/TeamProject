#include "shop.h"
#include"game.h"
#include<QString>
#include<QList>
#include<QDebug>
#include<QFile>
#include<QTextStream>
#include<QTextItem>
#include"textboard.h"
#include<QFont>
#include<QGraphicsPixmapItem>
extern Game* game;

Shop::Shop(QWidget *parent)
{
    this->setParent(game);
    scene = new QGraphicsScene(this);
    setBackgroundBrush(QBrush(QImage(":/Images/pics/backgound5.jpg")));

    //QGraphicsPixmapItem chicken();


    scene->setSceneRect(0,0,1000,1000);
    setScene(scene);
    setFixedSize(1000,1000);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
this->
    //钱
    money = new QGraphicsTextItem();
    money->setZValue(1);
    money->setFont(QFont("times", 15));
    money->setPlainText(QString("Money: ") + QString::number(game->possessions));
    money->setPos(0, 50);
    scene->addItem(money);

    n_bullet = new QGraphicsTextItem();
    n_bullet->setZValue(1);
    n_bullet->setFont(QFont("times", 15));
    n_bullet->setPlainText(QString("Bullets: ") + QString::number(game->weapon));
    n_bullet->setPos(0, 100);
    scene->addItem(n_bullet);
    n_bullet->setDefaultTextColor(Qt::blue);
    n_life = new QGraphicsTextItem();
    n_life->setZValue(1);
    n_life->setFont(QFont("times", 15));
    n_life->setPlainText(QString("你还有") + QString::number(game->life) + QString("条命"));
    n_life->setPos(0, 150);
    scene->addItem(n_life);

    //商店的标题
    Title = new QGraphicsTextItem("商店");
    Title->setPos(500 - Title->boundingRect().width()/2, 100);
    scene->addItem(Title);

    back = new Button("返回");
    back->setPos(500 - back->boundingRect().width()/2, 800);
    scene->addItem(back);

    //生命值，子弹，支持我们
    //商品名称，商品， 购买按钮

    Life = new TextBoard("");

    QGraphicsTextItem *text1 = new QGraphicsTextItem("买命");
    Life->setRect(0,0,250,500);
    Life->text->setPos(160 - Life->rect().width() / 2, 200);
    Life->setPos(160 - Life->rect().width() / 2, 200);

    QGraphicsPixmapItem *pic1 = new QGraphicsPixmapItem();
    pic1->setPixmap(QPixmap(":/Images/pics/BuyLife.jpg"));
    pic1->setPos(160 - pic1->boundingRect().width() / 2, 350);
    pic1->setZValue(2);
    this->scene->addItem(pic1);

    text1->setPos(160 - text1->boundingRect().width() / 2, 250);
    text1->setZValue(1);
    this->scene->addItem(Life);
    this->scene->addItem(text1);

    BuyLife = new Button("购买");
    BuyLife->setPos(180 - Life->rect().width() / 2,600);
    this->scene->addItem(BuyLife);


    Bullet = new TextBoard("");


    QGraphicsPixmapItem *pic2 = new QGraphicsPixmapItem();
    pic2->setPixmap(QPixmap(":/Images/pics/BuyBullet.jpg"));
    pic2->setPos(500 - pic2->boundingRect().width() / 2, 350);
    pic2->setZValue(2);
    this->scene->addItem(pic2);


    //子弹20元一个
    QGraphicsTextItem *text2= new QGraphicsTextItem("买子弹");
    QGraphicsTextItem *hint1 = new QGraphicsTextItem("子弹滞销，帮帮我们！\n \ \ \ \ \ \ 20元/个");
    Bullet->setRect(0,0,250,500);
    Bullet->text->setPos(500 - Bullet->rect().width() / 2, 200);
    Bullet->setPos(500 - Bullet->rect().width() / 2, 200);
    text2->setPos(500 - text2->boundingRect().width() / 2, 250);
    text2->setZValue(1);

    hint1->setPos(500 - hint1->boundingRect().width() / 2, 300);
    hint1->setZValue(1);

    this->scene->addItem(Bullet);
    this->scene->addItem(text2);
    this->scene->addItem(hint1);

    BuyBullet = new Button("购买");
    BuyBullet->setPos(500 - BuyBullet->rect().width() / 2,600);
    this->scene->addItem(BuyBullet);


    SupportUs = new TextBoard("");




    QGraphicsTextItem *text3= new QGraphicsTextItem("支持我们！");

    SupportUs->setRect(0,0,250,500);
    SupportUs->text->setPos(840 - SupportUs->rect().width() / 2, 200);
    SupportUs->setPos(840 - SupportUs->rect().width() / 2, 200);

    text3->setPos(840 - text3->boundingRect().width() / 2, 250);
    text3->setZValue(1);
    this->scene->addItem(SupportUs);
    this->scene->addItem(text3);

    b_SupportUs = new Button("资瓷！");
    b_SupportUs->setPos(840 - b_SupportUs->rect().width() / 2,600);
    this->scene->addItem(b_SupportUs);

    QGraphicsPixmapItem *pic3 = new QGraphicsPixmapItem();
    pic3->setPixmap(QPixmap(":/Images/pics/supportUs.gif"));
    pic3->setPos(840 - pic3->boundingRect().width() / 2, 350);
    pic3->setZValue(1);
    this->scene->addItem(pic3);


    connect(back, &Button::clicked, this, &Shop::hide);
    connect(BuyBullet, &Button::clicked, this, [=](){
        if(game->possessions > 20)
        {
            game->possessions -= 20;
            game->weapon ++;
            money->setPlainText(QString("Money: ") + QString::number(game->possessions));
            n_bullet->setPlainText(QString("Bullets: ") + QString::number(game->weapon));
            game->money->setPlainText(QString("Money: ") + QString::number(game->possessions));
            game->n_bullet->setPlainText(QString("Bullets: ") + QString::number(game->weapon));
            game->SaveBullets();
            game->SavePossessions();
        }
    });

    connect(b_SupportUs, &Button::clicked, this, &Shop::Support);
    connect(BuyLife, &Button::clicked, this, &Shop::Continue);

    this->hide();

}

void Shop::Support()
{
    pay = new QGraphicsTextItem("14762998710,微信支付宝同号:)");
    QFont font("times", 25);
    pay->setFont(font);
    pay->setDefaultTextColor(Qt::red);
    int xPos = this->x() + rect().width()/2 - pay->boundingRect().width()/2;
    int yPos = 900;
    pay->setPos(xPos, yPos);
    scene->addItem(pay);

}

void Shop::Continue()
{
    //40一条命
    if(game->possessions > 40)
    {
        game->possessions -= 40;
        game->life ++;
        money->setPlainText(QString("Money: ") + QString::number(game->possessions));
        n_life->setPlainText(QString("你还有") + QString::number(game->life) + QString("条命"));

        game->money->setPlainText(QString("Money: ") + QString::number(game->possessions));
        game->n_life->setPlainText(QString("你还有") + QString::number(game->life) + QString("条命"));
        game->SaveLife();
        game->SavePossessions();
    }

}
