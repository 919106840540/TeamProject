#ifndef TEXTBOARD_H
#define TEXTBOARD_H

#include <QObject>
#include <QGraphicsRectItem>
#include<QBrush>
#include<QGraphicsTextItem>

class TextBoard : public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    explicit TextBoard(QString name, QGraphicsItem * parent = NULL);
    QGraphicsTextItem *text;
    QBrush *brush;

signals:

public slots:
};

#endif // TEXTBOARD_H

