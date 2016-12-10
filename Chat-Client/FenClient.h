#ifndef FENCLIENT_H
#define FENCLIENT_H

#include <QDialog>
#include <QtNetwork>
#include <QMessageBox>
#include <QMainWindow>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QBoxLayout>
#include <QTextEdit>
#include <QMenuBar>
#include <QKeyEvent>
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include "FenDialog.h"

class FenClient : public QMainWindow
{
    Q_OBJECT

public:
    FenClient(QWidget *parent = 0);
private slots:
    void on_boutonConnexion_clicked();
    void on_boutonDeconnexion_clicked();
    void on_boutonEnvoyer_clicked();
    void on_boutonEffacer_clicked();
    void donneesRecues();
    void connecte();
    void deconnecte();
    void erreurSocket(QAbstractSocket::SocketError erreur);
    void about();
    void enregistrer();
private:
    QTcpSocket *socket;
    quint16 tailleMessage;

    QLabel *ip;
    QLabel *port;
    QLabel *pseudo;
    QLabel *message;

    QPushButton *boutonConnexion;
    QPushButton *boutonDeconnexion;
    QPushButton *boutonEnvoyer;
    QPushButton *boutonEffacer;
    QPushButton *boutonAbout;
    QPushButton *boutonQuitter;
    QPushButton *boutonSauvegarder;
    QPushButton *boutonChangerPseudo;

    QLineEdit *texteIP;
    QLineEdit *textePseudo;
    QLineEdit *texteMessage;

    QSpinBox *textePort;

    QTextEdit *listeMessages;

    QAction *actionConnexion;
    QAction *actionDeconnexion;
    QAction *actionEffacer;
};

#endif // FENCLIENT_H
