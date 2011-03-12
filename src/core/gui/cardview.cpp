#include "cardview.h"

#include <QtCore/QVariant>
#include <QtGui/QResizeEvent>
#include <QtGui/QGraphicsObject>
#include <QtDeclarative/QDeclarativeView>

#include <pictureloader.h>

class CardViewPrivate
{
public:
    Card card;
    QDeclarativeView *view;
    QString qmlSource;
    QGraphicsPixmapItem *pixmapItem;

    PictureLoader *loader;
};

CardView::CardView(QWidget *parent) :
    QWidget(parent),
    d_ptr(new CardViewPrivate)
{
    Q_D(CardView);

    d->loader = 0;
    d->view = new QDeclarativeView(this);
    d->pixmapItem = new QGraphicsPixmapItem;
    d->view->scene()->addItem(d->pixmapItem);
}

CardView::~CardView()
{
    delete d_ptr;
}

void CardView::setCard(const Card &card)
{
    Q_D(CardView);

    d->card = card;

    QMetaObject::invokeMethod(d->view->rootObject(), "setCard", Q_ARG(QVariant, card.attributes()));
    d->loader->getImage(card);
}

Card CardView::card() const
{
    return d_func()->card;
}

void CardView::resizeEvent(QResizeEvent *event)
{
    d_func()->view->resize(event->size());
}

void CardView::setQmlSource(const QString &file)
{
    Q_D(CardView);

    d->qmlSource = file;
    d->view->setSource(QUrl::fromLocalFile(file));
}

QString CardView::qmlSource()
{
    return d_func()->qmlSource;
}

void CardView::updateImage(const QString &id, const QString &path)
{
    Q_D(CardView);

    if (d->card.id() != id)
        return;

    d->pixmapItem->setPixmap(QPixmap(path));
}

void CardView::setPictureLoader(PictureLoader *loader)
{
    Q_D(CardView);

    if (d->loader) {
        disconnect(d->loader, SIGNAL(imageLoadedForCard(QString,QString)),
                   this,  SLOT(updateImage(QString,QString)));
    }
    d->loader = loader;
    connect(d->loader, SIGNAL(imageLoadedForCard(QString,QString)), SLOT(updateImage(QString,QString)));
}
