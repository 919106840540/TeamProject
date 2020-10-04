#include "game.h"
#include"shop.h"
#include"rank.h"
#include<QString>
#include<QList>
#include<QDebug>
#include<QFile>
#include<QTextStream>
#include<QtAlgorithms>
#include<QTableWidget>

extern Shop *shop;
//设计功能
//0. 记录下历史最高分，通过文件读写，刷新记录时重新写入 /完成
//2. 金币功能，后期增加商店功能  /存文本文档/完成
//3. 子弹功能/完成
//4. 设计音效/完成
//5. block，色泽/完成
//6. 平滑的上升 /完成
//7. 更合适的重力加速度 /完成


Game::Game(QWidget *parent)
{
    //初始化音频
    weapon = 0;
    life = 0;
    record1 = new int[10];
    fail_sound = new QMediaPlayer();
    fail_sound->setMedia(QUrl("qrc:/sound/media/2.wav"));
    LoadRecord();
    LoadBullets();
    LoadLife();


    //调用loadPossessions函数，获得已经有的金币数
    LoadPossessions();
    // create the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,1000);
    setScene(scene);

    setFixedSize(1000,1000);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    Welcome();
}

void Game::Welcome()
{
    setBackgroundBrush(QBrush(QImage(":/Images/pics/backgound4")));



    //金币数量
    money = new QGraphicsTextItem();
    money->setZValue(1);
    money->setFont(QFont("times", 15));
    money->setPlainText(QString("Money: ") + QString::number(this->possessions));
    money->setPos(0, 50);
    scene->addItem(money);

    n_bullet = new QGraphicsTextItem();
    n_bullet->setZValue(1);
    n_bullet->setFont(QFont("times", 15));
    n_bullet->setDefaultTextColor(Qt::blue);
    n_bullet->setPlainText(QString("Bullets: ") + QString::number(this->weapon));
    n_bullet->setPos(0, 100);
    scene->addItem(n_bullet);

    n_life = new QGraphicsTextItem();
    n_life->setZValue(1);
    n_life->setFont(QFont("times", 15));
    n_life->setPlainText(QString("你还有") + QString::number(this->life) + QString("条命"));
    n_life->setPos(0, 150);
    scene->addItem(n_life);



    //游戏标题
    titleText = new QGraphicsTextItem("Flappy Bird");
    titleText->setZValue(1);
    QFont titleFont("times" , 50);
    titleText->setFont( titleFont);
    int xPos = width()/2 - titleText->boundingRect().width()/2;
    int yPos = 150;
    titleText->setPos(xPos,yPos);
    this->scene->addItem(titleText);

    //开始标志
    playButton = new Button("Start!");
    int pxPos = width()/2 - playButton->boundingRect().width()/2;
    int pyPos = 500;
    playButton->setPos(pxPos,pyPos);
    this->scene->addItem(playButton);
    connect(playButton, &Button::clicked, this, Game::start);

    //前往商店
    front_shop = new Button("前往商店");
    int fxPos = width()/2 - playButton->boundingRect().width()/2;
    int fyPos = 650;
    front_shop->setPos(fxPos, fyPos);
    this->scene->addItem(front_shop);
    connect(front_shop, &Button::clicked, this, &Game::GoToShop);

    rank_button = new Button("排行榜");
    int rxPos = width()/2 - playButton->boundingRect().width()/2;
    int ryPos = 800;
    rank_button->setPos(rxPos, ryPos);
    this->scene->addItem(rank_button);

    Rank * rank = new Rank();
    connect(rank_button, &Button::clicked, rank, &Rank::show);
}


void Game::fail()
{
    //发出声音
    if (fail_sound->state() == QMediaPlayer::PlayingState){
        fail_sound->setPosition(0);
    }
    else if (fail_sound->state() == QMediaPlayer::StoppedState){
        fail_sound->play();
    }

    //游戏结束，停止增加分数
    disconnect(score_timer, &QTimer::timeout, score, &Score::increase);

    fail_board = new TextBoard("诶呀呀。。。");

    fail_board->setRect(0, 0, 600, 750);

    fail_board->setPos(500 - fail_board->rect().width() / 2, 100);
    this->scene->addItem(fail_board);


    button_main = new Button(QString("返回主菜单"));
    button_new = new Button(QString("重新开始"));
    button_shop = new Button("商店");

    button_main->setPos(500 - button_main->rect().width() / 2, 300);
    button_new->setPos(500 - button_new->rect().width() / 2, 450);
    button_shop->setPos(500 - button_new->rect().width() / 2, 600);

    this->scene->addItem(button_main);
    this->scene->addItem(button_new);
    this->scene->addItem(button_shop);

    if(this->life > 0)
    {
        Continue = new Button(QString("续1s"));
        Continue->setPos(500 - Continue->rect().width() / 2, 750);
        this->scene->addItem(Continue);
        connect(Continue, &Button::clicked, this, &Game::GoOn);
    }



    connect(button_main, &Button::clicked, this, &Game::BackToMainMenu);
    connect(button_new, &Button::clicked, this, &Game::NewGame);
    connect(button_shop, &Button::clicked, this, &Game::GoToShop);

    //记录下此时的分数，调用SaveRecord函数

    SaveRecord();
    SavePossessions();
    SaveBullets();
    SaveLife();

}

void Game::BackToMainMenu()
{

    scene->clear();
    this->Welcome();
}

void Game::NewGame()
{

    scene->clear();
    this->start();
}

void Game::start(){


    //用remov+delete的方式不安全，有可能出现重复删除的情况
    //因此用clear()方法，检测在Scene里的对象，移除并删除
    scene->clear();
    //显示分数
    money = new QGraphicsTextItem();
    money->setZValue(1);
    money->setFont(QFont("times", 15));
    money->setPlainText(QString("Money: ") + QString::number(this->possessions));
    money->setPos(0, 50);
    scene->addItem(money);

    n_bullet = new QGraphicsTextItem();
    n_bullet->setZValue(1);
    n_bullet->setFont(QFont("times", 15));
    n_bullet->setPlainText(QString("Bullets: ") + QString::number(this->weapon));
    n_bullet->setDefaultTextColor(Qt::blue);
    n_bullet->setPos(0, 100);
    scene->addItem(n_bullet);

    n_life = new QGraphicsTextItem();
    n_life->setZValue(1);
    n_life->setFont(QFont("times", 15));
    n_life->setPlainText(QString("你还有") + QString::number(this->life) + QString("条命"));
    n_life->setPos(0, 150);
    scene->addItem(n_life);


    score = new Score();
    score->setZValue(1);
    scene->addItem(score);
    score->setPos(0, 0);

    score_timer = new QTimer(this);
    connect(score_timer, &QTimer::timeout, score, &Score::increase);
    score_timer->start(500);



    birdy = new bird();
    birdy->setPos(100, 500);

    birdy->setFlag(QGraphicsItem::ItemIsFocusable);
    birdy->setFocus();

    scene->addItem(birdy);

    //只能放在这，因为到这一步birdy才被创建，前面识别不了信号的发送者
    connect(birdy, &bird::gameover, this, &Game::fail);
}

void Game::LoadRecord()
{
    QFile record_info("../flappy_bird/info/Record.txt");
    record_info.open(QIODevice::ReadOnly);
    QTextStream in(&record_info);

    while(!in.atEnd())//检测
    {
        //读取数据
        int dat;
        in >> dat;
        record.push_back(dat);
    }
    //排序
    qSort(record.begin(), record.end());
    record_info.close();
}



void Game::SaveRecord()
{
    QFile record_info("../flappy_bird/info/Record.txt");
    record_info.open(QFile::WriteOnly | QFile::Append);
    QTextStream out(&record_info);
    //日后改进为只存为前十个
    out << this->score->getscore() / 2 << "\n";

    //写下排行数据，追加方式

    record_info.close();

}

void Game::GoToShop()
{
    shop->show();
}

void Game::LoadPossessions()
{
    QFile Possessions_info("../flappy_bird/info/Possessions.txt");
    Possessions_info.open(QIODevice::ReadOnly);
    QTextStream in(&Possessions_info);

    if(!in.atEnd())//检测
    {
        //读取数据
        int temp;
        in >> temp;
        this->possessions = temp;
    }
    else
        this->possessions = 0;

    Possessions_info.close();
}

void Game::SavePossessions()
{
    QFile Possessions_info("../flappy_bird/info/Possessions.txt");
    Possessions_info.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&Possessions_info);

    out << this->possessions;

    Possessions_info.close();

}

void Game::SaveBullets()
{
    QFile Bullet_info("../flappy_bird/info/Bullet.txt");
    Bullet_info.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&Bullet_info);

    out << this->weapon;

    Bullet_info.close();
}

void Game::LoadBullets()
{
    QFile Bullet_info("../flappy_bird/info/Bullet.txt");
    Bullet_info.open(QIODevice::ReadOnly);
    QTextStream in(&Bullet_info);

    if(!in.atEnd())//检测
    {
        //读取数据
        int temp;
        in >> temp;
        this->weapon = temp;
    }
    else
        this->weapon = 0;

    Bullet_info.close();
}

void Game::LoadLife()
{
    QFile Life_info("../flappy_bird/info/Life.txt");
    Life_info.open(QIODevice::ReadOnly);
    QTextStream in(&Life_info);

    if(!in.atEnd())//检测
    {
        //读取数据
        int temp;
        in >> temp;
        this->life= temp;
    }
    else
        this->life = 0;

    Life_info.close();
}

void Game::SaveLife()
{
    QFile Life_info("../flappy_bird/info/Life.txt");
    Life_info.open(QIODevice::WriteOnly | QIODevice::Truncate);
    QTextStream out(&Life_info);

    out << this->life;

    Life_info.close();

}

void Game::GoOn()
{
//    emit Recover();
    //去除面板内容
    life--;
    n_life->setPlainText(QString("你还有") + QString::number(this->life) + QString("条命"));
    SaveLife();
    scene->removeItem(fail_board);
    scene->removeItem(button_main);
    scene->removeItem(button_new);
    scene->removeItem(button_shop);
    scene->removeItem(Continue);


    delete fail_board;
    delete button_main;
    delete button_new;
    delete button_shop;
    delete Continue;

    emit birdy->GoOn();
    connect(score_timer, &QTimer::timeout, score, &Score::increase);

}
