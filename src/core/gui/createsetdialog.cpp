#include "createsetdialog.h"
#include "ui_createsetdialog.h"

#include <QFileDialog>

CreateSetDialog::CreateSetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateSetDialog)
{
    ui->setupUi(this);
    connect(ui->fileDialogButton, SIGNAL(clicked()), SLOT(getFilePath()));
}

CreateSetDialog::~CreateSetDialog()
{
    delete ui;
}

QString CreateSetDialog::edition()
{
    return ui->editionLineEdit->text();
}

QString CreateSetDialog::editionName()
{
    return ui->editionNameLineEdit->text();
}

QString CreateSetDialog::description()
{
    return ui->descriptionTextEdit->toPlainText();
}

QString CreateSetDialog::path()
{
    return ui->pathLineEdit->text();
}

void CreateSetDialog::getFilePath()
{
    QString path = QFileDialog::getOpenFileName(this, "Select spoiler file");
    if (path.isEmpty())
        return;
    ui->pathLineEdit->setText(path);
}
