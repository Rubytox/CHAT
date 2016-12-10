#include "FenServeur.h"

using namespace std;

int const FenServeur::EXIT_CODE_REBOOT = -123456789;


FenServeur::FenServeur()
{
    setWindowIcon(QIcon("icone.png"));

    timeBeforeLeft = 6;
    timeBeforeLeftR = 6;
    m_time1s = new QTimer;
    m_time1sR = new QTimer;
    panelCommandes = new QLineEdit;
    afficheurCommandes = new QTextEdit;
    afficheurCommandes->setReadOnly(true);

    pseudo = new QLabel("Pseudo : ");
    message = new QLabel("Message : ");
    textePseudo = new QLineEdit;
    password = new QLabel("Mot de passe : ");
    textePassword = new QLineEdit;
    textePassword->setEchoMode(QLineEdit::Password);

    envoyerCommandes = new QPushButton("Envoyer");
    connect(envoyerCommandes, SIGNAL(clicked()), this, SLOT(on_boutonEnvoyer_clicked()));
    connect(panelCommandes, SIGNAL(returnPressed()), this, SLOT(on_boutonEnvoyer_clicked()));

    boutonValider = new QPushButton("Valider");
    connect(boutonValider, SIGNAL(clicked()), this, SLOT(verifierPassword()));
    connect(textePseudo, SIGNAL(returnPressed()), this, SLOT(verifierPassword()));
    connect(textePassword, SIGNAL(returnPressed()), this, SLOT(verifierPassword()));

    etatServeur = new QLabel;
    boutonQuitter = new QPushButton(tr("&Quitter"));
    connect(boutonQuitter, SIGNAL(clicked()), this, SLOT(timer()));
    connect(m_time1s, SIGNAL(timeout()), this, SLOT(quitter()));
    connect(this, SIGNAL(leave()), qApp, SLOT(quit()));

    boutonReboot = new QPushButton(tr("&Redémarrer"));
    connect(boutonReboot, SIGNAL(clicked()), this, SLOT(timerR()));
    connect(m_time1sR, SIGNAL(timeout()), this, SLOT(rebootTimer()));
    connect(this, SIGNAL(reboot()), this, SLOT(rebootServ()));


    nbClients = 0;

    texteNbClients = new QLabel;
    texteNbClients->setText(tr("Il y a actuellement <strong>") + QString::number(nbClients) + tr("</strong> client(s) connectés"));


    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(etatServeur);
    layout->addWidget(texteNbClients);
    layout->addWidget(afficheurCommandes);
    QHBoxLayout *layoutBoutons = new QHBoxLayout;
    QHBoxLayout *layoutID = new QHBoxLayout;
    layoutID->addWidget(pseudo);
    layoutID->addWidget(textePseudo);
    layoutID->addWidget(password);
    layoutID->addWidget(textePassword);
    layoutID->addWidget(boutonValider);
    QHBoxLayout *layoutCommandes = new QHBoxLayout;
    layoutCommandes->addWidget(message);
    layoutCommandes->addWidget(panelCommandes);
    layoutCommandes->addWidget(envoyerCommandes);
    layout->addLayout(layoutCommandes);
    layoutBoutons->addWidget(boutonQuitter);
    layoutBoutons->addWidget(boutonReboot);
    layout->addLayout(layoutID);
    layout->addLayout(layoutBoutons);

    setLayout(layout);

    setWindowTitle(tr("Chat - Serveur"));

    serveur = new QTcpServer(this);
    if(!serveur->listen(QHostAddress::Any, 50885))
    {
        etatServeur->setText(tr("Le serveur n'a pas pu être démarré. Raison :<br />") + serveur->errorString());
    }
    else
    {
        etatServeur->setText(tr("Le serveur a été démarré sur le port <strong>") + QString::number(serveur->serverPort()) + tr("</strong>.<br />Des clients peuvent maintenant se connecter."));
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }

    boutonQuitter->hide();
    boutonReboot->hide();
    envoyerCommandes->hide();
    panelCommandes->hide();
    etatServeur->hide();
    texteNbClients->hide();
    message->hide();
    textePseudo->setFocus();

    tailleMessage = 0;
}

void FenServeur::verifierPassword()
{
    QString pseudoHash = (QCryptographicHash::hash(textePseudo->text().toUtf8(), QCryptographicHash::Sha1)).toHex();
    QString passwordHash = (QCryptographicHash::hash(textePassword->text().toUtf8(), QCryptographicHash::Sha1)).toHex();
    if(pseudoHash == "f3ad7bdb294089b38a48ce47ac6d9daaf3a39c6f" && passwordHash == "b3c5a6d46acdd82bffafc4c498478e5e8a061560")
    {
        afficheurCommandes->append("<span style=\"color:green;\"><strong>Les identifiants sont corrects! Vous avez accès à la gestion du serveur!</strong></span>");

        boutonQuitter->show();
        boutonReboot->show();
        envoyerCommandes->show();
        panelCommandes->show();
        etatServeur->show();
        texteNbClients->show();
        message->show();

        textePseudo->hide();
        textePassword->hide();
        boutonValider->hide();
        pseudo->hide();
        password->hide();

        panelCommandes->setFocus();

        return;
    }
    else
    {
        afficheurCommandes->append("<span style=\"color:red;\"><strong>Le pseudo ou le mot de passe est incorrect!</strong></span>");
        return;
    }
}

void FenServeur::donneesRecues()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());

    if(socket == 0)
        return;

    QDataStream in(socket);

    if(tailleMessage == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> tailleMessage;
    }

    if(socket->bytesAvailable() < tailleMessage)
        return;

    QString message;
    in >> message;

    if(message == "/count")
    {
        envoyerNbClients();
    }
    else if(message.left(7) == "pseudo:")
    {
        pseudoNewConnexion = message.right(message.size() - 7);
    }
    else
    {
        envoyerATous(message);
    }

    tailleMessage = 0;
}


void FenServeur::nouvelleConnexion()
{
    nbClients++;

    QTcpSocket *nouveauClient = serveur->nextPendingConnection();

    clients << nouveauClient;

    if(pseudoNewConnexion == "")
    {
        envoyerATous(tr("<strong><span style=\"color:red;\">Serveur</span> : </strong> Un client vient de se connecter.<br /><strong><span style=\"color:red;\">Serveur</span> : </strong> Il y a actuellement ") + QString::number(nbClients) + tr(" client(s) connectés.<br />"));
    }
    else
    {
        envoyerATous(tr("<strong><span style=\"color:red;\">Serveur</span> : ") + pseudoNewConnexion + tr("</strong> vient de se connecter.<br /><strong><span style=\"color:red;\">Serveur</span> : </strong> Il y a actuellement ") + QString::number(nbClients) + tr(" client(s) connectés.<br />"));
    }
    texteNbClients->setText(tr("Il y a actuellement <strong>") + QString::number(nbClients) + tr("</strong> client(s) connectés"));

    connect(nouveauClient, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(nouveauClient, SIGNAL(disconnected()), this, SLOT(deconnexionClient()));
}

void FenServeur::deconnexionClient()
{
    nbClients--;
    envoyerATous(tr("<strong><span style=\"color:red;\">Serveur</span> : </strong> Un client vient de se déconnecter.<br /><strong><span style=\"color:red;\">Serveur</span> : </strong> Il y a actuellement ") + QString::number(nbClients) + tr(" client(s) connectés.<br />"));
    texteNbClients->setText(tr("Il y a actuellement <strong>") + QString::number(nbClients) + tr("</strong> client(s) connectés"));

    QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
    if(socket == 0)
        return;

    clients.removeOne(socket);
    socket->deleteLater();
}

void FenServeur::envoyerATous(const QString &message)
{
    afficheurCommandes->append(message);

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    out << (quint16) 0;
    out << message;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    for(int i = 0; i < clients.size(); i++)
        clients[i]->write(paquet);
}

void FenServeur::envoyerNbClients()
{
     envoyerATous("<strong><span style=\"color:red;\">Serveur</span> : </strong> Il y a actuellement <strong>" + QString::number(nbClients) + "</strong> client(s) connectés.");
}

void FenServeur::timer()
{
    m_time1s->start(1000);
    boutonReboot->setEnabled(false);
}

void FenServeur::quitter()
{
    if(nbClients != 0)
    {
        timeBeforeLeft--;
        QString messageQuit = "<strong><span style=\"color:red;\">Serveur</span> : </strong> Le serveur va s'éteindre dans <strong>" + QString::number(timeBeforeLeft) + "</strong> secondes!";
        envoyerATous(messageQuit);
        if(timeBeforeLeft == 0)
        {
            emit leave();
        }
    }
    else
    {
        emit leave();
    }
}

void FenServeur::timerR()
{
    m_time1sR->start(1000);
    boutonQuitter->setEnabled(false);
}

void FenServeur::rebootTimer()
{
    if(nbClients != 0)
    {
        timeBeforeLeftR--;
        QString messageReboot = "<strong><span style=\"color:red;\">Serveur</span> : </strong> Le serveur va redémarrer dans <strong>" + QString::number(timeBeforeLeftR) + "</strong> secondes!";
        envoyerATous(messageReboot);
        if(timeBeforeLeftR == 0)
        {
            emit reboot();
        }
    }
    else
    {
        emit reboot();
    }
}

void FenServeur::rebootServ()
{
    qDebug() << "Reboot en cours...";
    qApp->exit(FenServeur::EXIT_CODE_REBOOT);
}

void FenServeur::on_boutonEnvoyer_clicked()
{
    QString messageAEnvoyer = "<strong><span style=\"color:red;\">Serveur</span> : </strong>" + panelCommandes->text();
    if(panelCommandes->text().left(5) == "/say ")
    {
        QString variableFonction = panelCommandes->text().right(panelCommandes->text().size() - 5);
        afficheurCommandes->append(variableFonction);
        panelCommandes->clear();
        return;
    }
    else if(panelCommandes->text().left(5) == "/quit")
    {
        boutonQuitter->click();
        return;
    }
    else if(panelCommandes->text().left(7) == "/reboot")
    {
        boutonReboot->click();
        return;
    }
    else if(panelCommandes->text().left(6) == "/count")
    {
        envoyerNbClients();
        return;
    }
    else if(panelCommandes->text().left(4) == "/cls")
    {
        afficheurCommandes->clear();
        panelCommandes->clear();
        return;
    }
    else if(panelCommandes->text().left(5) == "/help")
    {
        QString helpTitre = tr("<div style=\"color:green;\"><br />=== LISTE DES COMMANDES SERVEUR ===<br />");
        QString help1 = tr("1. /say &lt;message&gt; => écrit &lt;message&gt;.<br />");
        QString help2 = tr("2. /count => Nombre de clients connectés.<br />");
        QString help3 = tr("3. /quit => éteint le serveur.<br />");
        QString help4 = tr("4. /reboot => relance le serveur.<br />");
        QString help5 = tr("5. /cls => efface les messages.<br />");
        QString helpFin = tr("</div>");
        QString messageHelp;
        messageHelp = helpTitre + help1 + help2 + help3 + help4 + help5 + helpFin;
        afficheurCommandes->append(messageHelp);
        panelCommandes->clear();
        return;
    }

    envoyerATous(messageAEnvoyer);

    panelCommandes->clear();
    panelCommandes->setFocus();
}
