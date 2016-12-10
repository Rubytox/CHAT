#include "FenDialog.h"

FenDialog::FenDialog()
{
    textePseudoDialog = new QLabel("Pseudo : ", this);
    champPseudoDialog = new QLineEdit;
    validerDialog = new QPushButton("Valider", this);
    quitterDialog = new QPushButton("Quitter", this);
    vboxDialog = new QVBoxLayout(this);
    hboxDialog = new QHBoxLayout(this);
    hboxDialog->addWidget(validerDialog);
    hboxDialog->addWidget(quitterDialog);
    vboxDialog->addWidget(textePseudoDialog);
    vboxDialog->addWidget(champPseudoDialog);
    vboxDialog->addLayout(hboxDialog);

    champPseudoDialog->setFocus();

    connect(quitterDialog, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(validerDialog, SIGNAL(clicked()), this, SLOT(verifier()));
    connect(this, SIGNAL(quitter()), this, SLOT(close()));

    setLayout(vboxDialog);
}

void FenDialog::verifier()
{
    if(!champPseudoDialog->text().isEmpty())
    {
        emit quitter();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "<strong>ERREUR:</strong> vous n'avez pas fourni de pseudo!");
    }
}
