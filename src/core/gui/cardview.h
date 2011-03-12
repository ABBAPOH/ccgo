#ifndef CARDVIEW_H
#define CARDVIEW_H

#include <QtGui/QWidget>

#include <card.h>

class PictureLoader;
class CardViewPrivate;
class CardView : public QWidget
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(CardView)
public:
    explicit CardView(QWidget *parent = 0);
    ~CardView();

    void setCard(const Card &card);
    Card card() const;

    void setQmlSource(const QString &file);
    QString qmlSource();

    void setPictureLoader(PictureLoader *loader);

signals:

private slots:
    void updateImage(const QString &id, const QString &path);

protected:
    virtual void resizeEvent(QResizeEvent *);

protected:
    CardViewPrivate *d_ptr;
};

#endif // CARDVIEW_H
