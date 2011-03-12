#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtSql/QSqlTableModel>
#include <QFileDialog>

#include <QDebug>
#include <QXmlSimpleReader>
#include <iohandlers/simplexmlhandler.h>
#include <iohandlers/simplexmliohandler.h>
#include <iohandlers/xmliohandler.h>
#include <cardbase.h>

#include <deck.h>
#include "deckmodel.h"

#include <QMessageBox>
#include <QTime>

#include "manacostdelegate.h"
#include <QSortFilterProxyModel>

#include <pictureloader.h>
#include "cardbaseproxymodel.h"

#include <game.h>

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

    connect(ui->filterLineEdit, SIGNAL(textChanged(QString)), SLOT(onFilter(QString)));

    connect(ui->actionSave_Deck, SIGNAL(triggered()), SLOT(saveDeck()));
    connect(ui->actionImport_CardBase, SIGNAL(triggered()), SLOT(importBase()));
    connect(ui->actionExport_CardBase, SIGNAL(triggered()), SLOT(exportBase()));

    connect(ui->actionClose, SIGNAL(triggered()), SLOT(close()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadWindow()
{
    qDebug() << "loadWindow";

    QTime time;
    time.start();

    game = new Game("magic", this);
    cardBase = new CardBase(game);
    loader = new PictureLoader(game);
    ui->cardView->setPictureLoader(loader);
    cardBase->addHandler(new XmlIOHandler);

    model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM cardbase");

    proxy = new CardBaseProxyModel(this);
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->cardBaseView->setModel(proxy);
    ui->cardBaseView->hideColumn(0);
//    Q_ASSERT(ui->cardBaseView->selectionModel());
    connect(ui->cardBaseView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            SLOT(onCurrentRowChange(QModelIndex,QModelIndex)));

    deck = new Deck(cardBase, this);
    deckModel = new DeckModel(deck, this);
    ui->deckView->setModel(deckModel);

    int i = 0;
    while (model->canFetchMore()) {
        model->fetchMore();
        if (i++%5 == 4)
        qApp->processEvents();
    }

    qDebug("Time elapsed: %d ms", time.elapsed());
}

void MainWindow::saveDeck()
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
        deck->addCard("Main Deck", this->cardBase->card(row.data().toString()));
    }
}

void MainWindow::addCardSB()
{
    QItemSelectionModel *selectionModel = ui->cardBaseView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        deck->addCard("Sideboard", cardBase->card(row.data().toString()));
    }
}

void MainWindow::removeCard()
{
    QItemSelectionModel *selectionModel = ui->deckView->selectionModel();

    foreach (QModelIndex row, selectionModel->selectedRows()) {
        QModelIndex groupIndex = row.parent();
        if (groupIndex.isValid()) {
            QModelIndex idx = deckModel->index(row.row(), 2, groupIndex);
            qDebug() << idx << idx.data() << groupIndex.data();
            deck->removeCard(groupIndex.data().toString(), cardBase->card(idx.data().toString()));
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
    }
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

void MainWindow::onCurrentRowChange(const QModelIndex &current, const QModelIndex &previous)
{
    QModelIndex sourceIndex = proxy->mapToSource(current);
    QModelIndex index = model->index(sourceIndex.row(), 0, sourceIndex.parent());
    ui->cardView->setCard(cardBase->card(index.data().toString()));
}
