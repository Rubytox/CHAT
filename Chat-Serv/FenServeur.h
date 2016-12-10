#ifndef FENSERVEUR_H
#define FENSERVEUR_H

#include <QApplication>
#include <QtNetwork>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QTimer>
#include <QMessageBox>
#include <QTextEdit>
#include <QCryptographicHash>

class FenServeur : public QWidget
{
    Q_OBJECT

public:
    static int const EXIT_CODE_REBOOT;

    FenServeur();
    //~FenServeur();
    void envoyerATous(const QString &message);
    void envoyerNbClients();
public slots:
    void nouvelleConnexion();
    void donneesRecues();
    void deconnexionClient();
    void timer();
    void timerR();
    void quitter();
    void rebootTimer();
    void rebootServ();
    void on_boutonEnvoyer_clicked();
    void verifierPassword();
signals:
    void leave();
    void reboot();
private:
    QLabel *etatServeur;
    QLabel *texteNbClients;
    QPushButton *boutonQuitter;
    QPushButton *boutonReboot;

    QTcpServer *serveur;
    QList<QTcpSocket *> clients;
    quint16 tailleMessage;

    int nbClients;
    int timeBeforeLeft;
    int timeBeforeLeftR;
    QTimer *m_time1s;
    QTimer *m_time1sR;

    QLineEdit *panelCommandes;
    QTextEdit *afficheurCommandes;
    QPushButton *envoyerCommandes;
    QLabel *pseudo;
    QLabel *password;
    QLabel *message;
    QLineEdit *textePseudo;
    QLineEdit *textePassword;
    QPushButton *boutonValider;

    QString pseudoNewConnexion;
};


#endif // FENSERVEUR_H
