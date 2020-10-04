#include "mainwindow.h"
#include <QApplication>
#include"game.h"
#include"shop.h"

Game *game;
Shop *shop;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game = new Game();
    shop = new Shop();

    game->show();



    return a.exec();
}
