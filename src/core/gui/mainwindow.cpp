#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql/QSqlQueryModel>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QMetaProperty>

#include <QDebug>

#include <game.h>
#include <cardbase.h>
#include <deck.h>
#include <edition.h>
#include <pictureloader.h>
#include <iohandlers/xmliohandler.h>
#include <iohandlers/spoileriohandler.h>

#include "cardbaseproxymodel.h"
#include "manacostdelegate.h"
#include "deckmodel.h"
#include "createsetdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->cardBaseView->setSortingEnabled(true);
    ui->cardBaseView->setItemDelegateForColumn(7, new ManaCostDelegate);
//    ui->cardBaseView->verticalHeader()->hide();

    ui->cardView->setQmlSource("/Users/arch/Programming/qt4/ccgo/src/CardView.qml");
//    ui->cardView->setCard(cardBase->card("[OD] Aether Burst"));

    connect(ui->addCardButton, SIGNAL(clicked()), SLOT(addCard()));
    connect(ui->addCardSBButton, SIGNAL(clicked()), SLOT(addCardSB()));
    connect(ui->removeCardButton, SIGNAL(clicked()), SLOT(removeCard()));
    connect(ui->removeAllCardsButton, SIGNAL(clicked()), SLOT(removeAllCards()));

    connect(ui->filterLineEdit, SIGNAL(textChanged(QString)), SLOT(onFilter(QString)));

    connect(ui->actionOpen_Deck, SIGNAL(triggered()), SLOT(openDeck()));
    connect(ui->actionSave_Deck_As, SIGNAL(triggered()), SLOT(saveDeckAs()));
    connect(ui->actionImport_CardBase, SIGNAL(triggered()), SLOT(importBase()));
    connect(ui->actionExport_CardBase, SIGNAL(triggered()), SLOT(exportBase()));
    connect(ui->actionClear_Base, SIGNAL(triggered()), SLOT(clearBase()));

    connect(ui->actionClose, SIGNAL(triggered()), SLOT(close()));

    connect(ui->actionAdd_Set, SIGNAL(triggered()), SLOT(addSet()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadWindow()
{
    game = new Game("magic", this);
    cardBase = new CardBase(game);
    loader = new PictureLoader(game);
    ui->cardView->setPictureLoader(loader);
    cardBase->addHandler(new XmlIOHandler);
//    cardBase->addHandler(new SpoilerIOHandler);

    model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM cardbase");

    proxy = new CardBaseProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->cardBaseView->horizontalHeader()->setStretchLastSection(true);
    ui->cardBaseView->setModel(proxy);
    ui->cardBaseView->hideColumn(0);
    for (int i = 3; i < model->columnCount(); i++) {
//        ui->cardBaseView->hideColumn(i);
    }
    ui->cardBaseView->verticalHeader()->hide();
    ui->cardBaseView->setColumnWidth(1, 200);
    ui->cardBaseView->setColumnWidth(2, 50);
    connect(ui->cardBaseView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onCurrentRowChange(QModelIndex,QModelIndex)));

    deck = new Deck(cardBase, this);
    deckModel = new DeckModel(deck, this);
    ui->deckView->setModel(deckModel);
    ui->deckView->hideColumn(1);
    ui->deckView->setAlternatingRowColors(true);

    int i = 0;
    while (model->canFetchMore()) {
        model->fetchMore();
        if (i++%5 == 4)
        qApp->processEvents();
    }
}

void MainWindow::onCurrentRowChange(const QModelIndex &current, const QModelIndex &previous)
{
    QModelIndex sourceIndex = proxy->mapToSource(current);
    QModelIndex index = model->index(sourceIndex.row(), 0, sourceIndex.parent());
    ui->cardView->setCard(cardBase->card(index.data().toString()));
}

void MainWindow::openDeck()
{
    QString file = QFileDialog::getOpenFileName(this, "Open Deck", "", "*.mwdeck");
    if (file.isEmpty())
        return;
    deck->load(file);
    ui->deckView->expandAll();
}

void MainWindow::saveDeckAs()
{
    QString file = QFileDialog::getSaveFileName();
    if (file.isEmpty())
        return;
    deck->save(file);
}

void MainWindow::addCard()
{
    QItemSelectionModel *selectionModel = ui->cardBaseView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        deck->addCard(cardBase->card(row.data().toString()), "Main Deck");
    }
    ui->deckView->expandAll();
}

void MainWindow::addCardSB()
{
    QItemSelectionModel *selectionModel = ui->cardBaseView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        deck->addCard(cardBase->card(row.data().toString()), "Sideboard");
    }
    ui->deckView->expandAll();
}

void MainWindow::removeCard()
{
    QItemSelectionModel *selectionModel = ui->deckView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        QModelIndex groupIndex = deckModel->topGroupIndex(row);
        if (groupIndex.isValid()) {
            QModelIndex idx = deckModel->index(row.row(), 1, row.parent());
            qDebug() << idx << idx.data() << groupIndex.data();
            deck->removeCard(cardBase->card(idx.data().toString()), groupIndex.data().toString());
        }
    }
}

void MainWindow::removeAllCards()
{
    QItemSelectionModel *selectionModel = ui->deckView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        QModelIndex groupIndex = deckModel->topGroupIndex(row);
        if (groupIndex.isValid()) {
            QModelIndex idx = deckModel->index(row.row(), 1, row.parent());
            qDebug() << idx << idx.data() << groupIndex.data();
            deck->removeAllCards(cardBase->card(idx.data().toString()), groupIndex.data().toString());
        }
    }
}

void MainWindow::onFilter(const QString &name)
{
    proxy->setNameFilter(name);
}

void MainWindow::importBase()
{
    QString file = QFileDialog::getOpenFileName(this, "Import As");
    if (file.isEmpty())
        return;
    if (!cardBase->importBase(file)) {
        QMessageBox::warning(0, ("Import Failed"),
                             QString("Cannot import base from file %1.")
                             .arg(file));
        return;
    }
    model->setQuery("SELECT * FROM cardbase");
    while (model->canFetchMore())
        model->fetchMore();
}

void MainWindow::exportBase()
{
    QString file = QFileDialog::getSaveFileName(this, "Export As");
    if (file.isEmpty())
        return;
    if (!cardBase->exportBase(file)) {
        QMessageBox::warning(0, ("Export Failed"),
                             QString("Cannot export base to file %1.")
                             .arg(file));
    }
}

void MainWindow::clearBase()
{
    cardBase->clearBase();
    model->setQuery("SELECT * FROM cardbase");
}

void MainWindow::addSet()
{
    CreateSetDialog dialog;

    if (!dialog.exec()) {
        return;
    }

    QString edition = dialog.edition();
    QString editionName = dialog.editionName();
    QString description = dialog.description();
    QString path = dialog.path();
    qDebug() << path << edition << editionName << description;

    SpoilerIOHandler handler;
    handler.setEdition(edition, editionName);
    if (handler.canHandle(path)) {
        bool result = handler.read(path, cardBase);
        if (!result) {
            qDebug() << "failed to read from spoiler";
        }
    }
}
