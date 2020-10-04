#ifndef BLOCK_H
#define BLOCK_H
#include<QTimer>
#include <QGraphicsRectItem>
#include <QObject>
#include <QGraphicsItem>
#include<QBrush>


class Block : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Block(QGraphicsItem *parent = nullptr);
    QTimer *timer;


    int height;
    int width;


signals:

public slots:
    void move();
    void stop();
    void ReConnect();
};

#endif // BLOCK_H



