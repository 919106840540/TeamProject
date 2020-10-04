#include "bird.h"
#include<QTimer>
#include <QKeyEvent>
#include<QGraphicsScene>
#include<QDebug>
#include <stdlib.h>
#include<QTimeLine>
#include<QGraphicsItemAnimation>
#include<QBrush>
#include"gold.h"
#include"bullet.h"
#include"game.h"

extern Game* game;

bird::bird(QGraphicsItem *parent): QGraphicsPixmapItem(parent)
{
    //载入音频，1为芜湖
    jump_sound = new QMediaPlayer();
    jump_sound->setMedia(QUrl("qrc:/sound/media/1.wav"));

    this->size = 40;
    time_since_clicked = 0;
    move_frame = 0;
    setPixmap(QPixmap(":/Images/pics/mainmain.png"));
    //    一定，一定，一定，一定指定它的parents
    //    ！！！！！！！！！！！！！！
    gravity_timer = new QTimer(this);
    gravity_timer->start(1);
    move_timer = new QTimer(this);

    connect(gravity_timer, &QTimer::timeout, [=](){
        this->time_since_clicked ++;
    });
    //这边没断开，导致速度极快


    //    this->setRect(0, 0, this->size, this->size);
    timer = new QTimer(this);
    //实现更平滑的下落，timer的间隔再缩短，同时减少下落速度
    //
    connect(timer, SIGNAL(timeout()), this, SLOT(gravity()));
    timer->start(20);

    spawn_timer  = new QTimer(this);
    //2秒生成一个block
    spawn_timer->start(2000);
    connect(this, &bird::keyPressed, this, &bird::ready);
    connect(spawn_timer, &QTimer::timeout, this, &bird::spawn);

    //检测是否撞上blocks
    connect(this, &bird::gameover, this, &bird::stop_spawn);
    connect(this, &bird::gameover, this, &bird::stop_move);

    coin_timer = new QTimer(this);
    connect(coin_timer, &QTimer::timeout, this, &bird::MakeCoins);
    coin_timer->start(2000);
    connect(this, &bird::gameover, this, &bird::StopMakeCoins);

    connect(this, &bird::GoOn, this, &bird::ReConnect);

}

void bird::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Space)
    {

        emit keyPressed();

        if (jump_sound->state() == QMediaPlayer::PlayingState){
            jump_sound->setPosition(0);
        }
        else if (jump_sound->state() == QMediaPlayer::StoppedState){
            jump_sound->play();
        }
    }
    if(event->key() == Qt::Key_E)
    {
        shoot();
    }
}

void bird::move()
{


    move_frame = 0;

    connect(move_timer, &QTimer::timeout, this, [=](){
        move_frame += 1;

    });

    connect(move_timer, &QTimer::timeout, this, [=](){
        if(move_frame < 150)
            this->setPos(this->x(), this->y() - 1);
    });

    move_timer->start(1);

}

void bird::spawn()
{
    //两个柱子之间的间隔为
    int seperate = 325;
    //随机生成上面柱子的长度，小于725

    int upper_height = rand() % (1000 - seperate);
    int lower_height = 1000 - seperate - upper_height;
    Block *block1 = new Block();
    block1->setRect(0, 0, block1->width, upper_height);

    scene()->addItem(block1);

    Block *block2 = new Block();
    block2->setRect(0, 0, block2->width, lower_height);
    block2->setPos(1000, 1000 - lower_height);
    scene()->addItem(block2);
    //qDebug() << "产生";
}

void bird::stop_move()
{
    disconnect(timer, SIGNAL(timeout()), this, SLOT(gravity()));
    disconnect(this, &bird::keyPressed, this, &bird::ready);
}

void bird::stop_spawn()
{
    disconnect(spawn_timer, &QTimer::timeout, this, &bird::spawn);
}

void bird::MakeCoins()
{
    //3/2的概率产生金币
    int whether_make_coins = rand() % 3;
    if(whether_make_coins < 2)
    {
        int ypos = rand() % 1000;
        gold *coin = new gold();
        //修改这里，防止重合
        coin->setPos(1000 - 150 - coin->boundingRect().width(), ypos);
        scene()->addItem(coin);
    }

}

void bird::StopMakeCoins()
{
    disconnect(coin_timer, &QTimer::timeout, this, &bird::MakeCoins);

}

void bird::shoot()
{
    qDebug() << game->weapon;
    if(game->weapon > 0)
    {
        Bullet *bullet = new Bullet();
        bullet->setPos(this->x(), this->y());
        scene()->addItem(bullet);
        game->weapon --;
        game->n_bullet->setPlainText(QString("Bullets: ") + QString::number(game->weapon));
        game->SaveBullets();
    }

}

void bird::ReConnect()
{
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFocus();
    this->time_since_clicked = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT(gravity()));

    connect(this, &bird::keyPressed, this, &bird::ready);
    connect(spawn_timer, &QTimer::timeout, this, &bird::spawn);
    connect(coin_timer, &QTimer::timeout, this, &bird::MakeCoins);

    setPos(this->x(), 0);
    for(;;)
    {
        QList<QGraphicsItem *> colliding_items = this->collidingItems();
        if(colliding_items.size() > 0)
            setPos(this->x(), this->y() + 15);
        else
            break;
    }
}

void bird::ready()
{
    this->move();
    this->time_since_clicked = 0;

}


void bird::gravity()
{
    //产生重力的效果，和上一次点击事件发生的时间有关
    //先先有一个初速度，或者提高计时器灵敏度
    int speed = this->time_since_clicked / 50;
    int v0 = 5;
    this->setPos(this->x(), this->y() + speed + v0);


    QList<QGraphicsItem *> colliding_items = this->collidingItems();
    // 检测碰撞
    for (int i = 0, n = colliding_items.size(); i < n; ++i){
        if (typeid(*(colliding_items[i])) == typeid(Block)){
            emit gameover();
        }
    }
    if(this->y() > 1000 - this->boundingRect().height())
        emit gameover();

}
