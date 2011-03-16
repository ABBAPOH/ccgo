#ifndef CREATESETDIALOG_H
#define CREATESETDIALOG_H

#include <QDialog>

namespace Ui {
    class CreateSetDialog;
}

class CreateSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSetDialog(QWidget *parent = 0);
    ~CreateSetDialog();

    QString edition();
    QString editionName();
    QString description();
    QString path();

private slots:
    void getFilePath();

private:
    Ui::CreateSetDialog *ui;
};

#endif // CREATESETDIALOG_H
