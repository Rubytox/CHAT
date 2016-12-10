#ifndef FENDIALOG_H
#define FENDIALOG_H

#include <QApplication>
#include <QDialog>
#include <QBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include "FenClient.h"

class FenDialog : public QDialog
{
    Q_OBJECT

public:
    FenDialog();
public slots:
    void verifier();
signals:
    void quitter();
private:
    QLabel *textePseudoDialog;
    QLineEdit *champPseudoDialog;
    QPushButton *validerDialog;
    QPushButton *quitterDialog;
    QVBoxLayout *vboxDialog;
    QHBoxLayout *hboxDialog;

    friend class FenClient;
};

#endif // FENDIALOG_H
