#include "manacostdelegate.h"

#include <QtGui/QPainter>
#include <QDir>
#include <QDebug>
#include <math.h>

ManaCostDelegate::ManaCostDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
//    if (pixmaps.isEmpty()) {
//        pixmaps.append(QPixmap("/Users/arch/Programming/qt4/ccgo/src/Resources/Mana/W.png").scaled(20, 20));
//    }

    QDir dir("/Users/arch/Programming/qt4/ccgo/resources/games/magic/Mana");
    foreach (QString entry, dir.entryList(QStringList() << "*.png")) {
        QFileInfo info(dir.absoluteFilePath(entry));
//        qDebug() << entry << info.baseName() << info.filePath();
        pixmaps.insert(info.baseName(), QPixmap(info.filePath()).scaled(20, 20));
    }

}

void ManaCostDelegate::paint(QPainter *painter,
                             const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
//    qDebug() << index.data();
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
//    painter->setPen(Qt::NoPen);

//    if (mode == Editable) {
//        painter->setBrush(palette.highlight());
//    } else {
//        painter->setBrush(palette.foreground());
//    }

    int scalingFactor = 20.0;
    QRect rect = option.rect;
    int yOffset = (rect.height() - scalingFactor ) / 2;
    painter->translate(rect.x(), rect.y() + yOffset);
//    painter->drawRect(0,0, 20, 20);
    QString data = index.data().toString();
    for (int i = 0; i < data.length(); i++) {
        painter->drawPixmap(20*i, 0, pixmaps[data.at(i)]);
    }

//    painter->scale(scalingFactor, scalingFactor);

//    QPolygonF starPolygon;
//    starPolygon << QPointF(1.0, 0.5);
//    for (int i = 1; i < 5; ++i)
//        starPolygon << QPointF(0.5 + 0.5 * cos(0.8 * i * 3.14),
//                               0.5 + 0.5 * sin(0.8 * i * 3.14));

//    for (int i = 0; i < 5; ++i) {
////        if (i < 3) {
//            painter->drawPolygon(starPolygon, Qt::WindingFill);
////        } else /*if (mode == Editable)*/ {
////            painter->drawPolygon(starPolygon, Qt::WindingFill);
////        }
//        painter->translate(1.0, 0.0);
//    }

    painter->restore();
}
