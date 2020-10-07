#include "textboard.h"
#include<QFont>
TextBoard::TextBoard(QString name, QGraphicsItem *parent) : QGraphicsRectItem(parent)
{
    //绘制正方形
    //确保悬浮在上面
    setZValue(1);
    setRect(0,0,600,350);
    brush = new QBrush();
    brush->setStyle(Qt::SolidPattern);
    brush->setColor(QColor(244,164,96));
    setBrush(*brush);

    //draw Text
    text = new QGraphicsTextItem(name,this);
    QFont font("times", 15);
    text->setFont(font);

    int xPos = this->x() + rect().width()/2 - text->boundingRect().width()/2;

    //居中
//    int yPos = rect().height()/2 - text->boundingRect().height()/2;
    //顶头
    int yPos = 50 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);
    text->setDefaultTextColor(Qt::blue);
}

