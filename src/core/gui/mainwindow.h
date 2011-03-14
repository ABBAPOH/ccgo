#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

namespace Ui {
    class MainWindow;
}

class CardBase;
class Deck;
class Game;
class DeckModel;
class QSqlQueryModel;
class PictureLoader;
class QModelIndex;
class CardBaseProxyModel;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void loadWindow();
    void openDeck();
    void saveDeckAs();
    void addCard();
    void addCardSB();
    void removeCard();
    void removeAllCards();
    void onFilter(const QString &name);

    void importBase();
    void exportBase();
    void clearBase();


    void onCurrentRowChange(const QModelIndex & current, const QModelIndex & previous);

private:
    Ui::MainWindow *ui;

    Game *game;
    CardBase *cardBase;
    Deck *deck;

    QSqlQueryModel *model;
    CardBaseProxyModel *proxy;
    DeckModel *deckModel;

    PictureLoader *loader;
};

class Class: public QObject
{
    Q_OBJECT
    Q_ENUMS(Qt::PenStyle)
    Q_ENUMS(A)
public:
    enum A{ a, b ,c};


};

#endif // MAINWINDOW_H
