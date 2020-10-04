#ifndef BIRD_H
#define BIRD_H

#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
#include "block.h"
#include <QTimer>
#include<QGraphicsPixmapItem>
#include<QMediaPlayer>

class bird : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    int size;
    int time_since_clicked;
    QTimer *gravity_timer;
    QTimer *spawn_timer;
    QTimer *move_timer;
    QTimer * timer;
    QTimer * coin_timer;
    int  move_frame;
    QMediaPlayer *jump_sound;



    bird(QGraphicsItem * parent=0);
    void keyPressEvent(QKeyEvent * event);


signals:
    void gameover();
    void keyPressed();
    void GoOn();




public slots:
//    void spawn();
    void gravity();
    void move();
    void spawn();
    void stop_move();
    void stop_spawn();
    void MakeCoins();
    void StopMakeCoins();
    void shoot();
    void ReConnect();

    void ready();


};

#endif // BIRD_H



