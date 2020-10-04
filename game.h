#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QWidget>
#include <QGraphicsScene>
#include "bird.h"
#include "block.h"
#include"rank.h"
#include"button.h"
#include<QTimer>
#include<QGraphicsTextItem>
#include"textboard.h"
#include"score.h"
#include"coins.h"
#include<QMediaPlayer>
#include<QList>
#include<QObject>

class Game :public QGraphicsView
{
public:

    Game(QWidget * parent=0);
    //开始界面
    void Welcome();
    //输了的界面
    //包括清屏，重新开始
    void fail();
    void BackToMainMenu();
    void NewGame();

    QGraphicsScene * scene;

    QList<QGraphicsItem*> list;

    void start();
    void LoadRecord();
    void SaveRecord();
    void GoToShop();
    void LoadPossessions();
    void SavePossessions();
    void SaveBullets();
    void LoadBullets();
    void ShowRank();
    void LoadLife();
    void SaveLife();
    void GoOn();


    Score *score;
    bird* birdy;
    QGraphicsTextItem *titleText;
    Button * playButton;
    TextBoard * fail_board;
    Button * button_main;
    Button * button_new;
    QTimer *score_timer;
    Button *button_shop;
    Button *front_shop;
    Button *rank_button;
    Button *Continue;
    int possessions;
    QMediaPlayer *fail_sound;

    QGraphicsTextItem *money;
    QGraphicsTextItem *n_bullet;
    QGraphicsTextItem *n_life;

    QList<int> record;
    int * record1;
    int weapon;
    int life;


signals:
    void Recover();


public slots:

};

#endif // GAME_H


