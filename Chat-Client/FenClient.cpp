#include "FenClient.h"
#include "FenDialog.h"

FenClient::FenClient(QWidget *parent) : QMainWindow(parent)
{
    QWidget *zoneCentrale = new QWidget;

    QIcon icone("icone.png");
    setWindowIcon(icone);


    FenDialog dialogPseudo;
    dialogPseudo.exec();

    socket = new QTcpSocket(this);

    setFixedSize(697, 467);

    ip = new QLabel("Adresse IP : ");
    port = new QLabel("Port : ");
    pseudo = new QLabel("Pseudo : ");
    message = new QLabel("Message : ");

    boutonConnexion = new QPushButton("Connexion");
    boutonDeconnexion = new QPushButton("Déconnexion");
    boutonEnvoyer = new QPushButton("Envoyer");
    boutonEffacer = new QPushButton("Effacer les messages");
    boutonAbout = new QPushButton("A propos");
    boutonQuitter = new QPushButton("Quitter");
    boutonSauvegarder = new QPushButton("Sauvegarder un log");
    boutonChangerPseudo = new QPushButton("Changer de pseudo", this);

    texteIP = new QLineEdit("192.168.0.14");
    textePseudo = new QLineEdit;
    texteMessage = new QLineEdit;

    textePseudo->setText(dialogPseudo.champPseudoDialog->text());
    textePseudo->setEnabled(false);
    pseudo->setEnabled(false);

    textePort = new QSpinBox;
    textePort->setRange(0, 65536);
    textePort->setValue(50885);

    listeMessages = new QTextEdit;
    listeMessages->setReadOnly(true);

    QHBoxLayout *layoutConnexion = new QHBoxLayout;
    layoutConnexion->addWidget(ip);
    layoutConnexion->addWidget(texteIP);
    layoutConnexion->addWidget(port);
    layoutConnexion->addWidget(textePort);
    layoutConnexion->addWidget(boutonConnexion);
    layoutConnexion->addWidget(boutonDeconnexion);

    QHBoxLayout *layoutMessage = new QHBoxLayout;
    layoutMessage->addWidget(pseudo);
    layoutMessage->addWidget(textePseudo);
    layoutMessage->addWidget(boutonChangerPseudo);
    layoutMessage->addWidget(message);
    layoutMessage->addWidget(texteMessage);
    layoutMessage->addWidget(boutonEnvoyer);

    QHBoxLayout *layoutPlus = new QHBoxLayout;
    layoutPlus->addWidget(boutonEffacer);
    layoutPlus->addWidget(boutonSauvegarder);
    layoutPlus->addWidget(boutonAbout);
    layoutPlus->addWidget(boutonQuitter);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(layoutConnexion);
    layout->addWidget(listeMessages);
    layout->addLayout(layoutMessage);
    layout->addLayout(layoutPlus);

    QMenu *menuFichier = menuBar()->addMenu("&Fichier");
    QMenu *menuEdition = menuBar()->addMenu("&Edition");
    QMenu *menuAffichage = menuBar()->addMenu("&Affichage");

    actionConnexion = new QAction("Se &connecter", this);
    actionConnexion->setShortcut(QKeySequence("Ctrl+N"));
    actionDeconnexion = new QAction("Se &déconnecter", this);
    actionDeconnexion->setShortcut(QKeySequence("Ctrl+W"));
    QAction *actionQuitter = new QAction("&Quitter", this);
    actionQuitter->setShortcut(QKeySequence("Ctrl+Q"));
    menuFichier->addAction(actionConnexion);
    menuFichier->addAction(actionDeconnexion);
    menuFichier->addSeparator();
    menuFichier->addAction(actionQuitter);

    actionEffacer = new QAction("&Effacer les messages", this);
    menuEdition->addAction(actionEffacer);

    QAction *actionCacherConnexion = new QAction("Cac&her la partie connexion", this);
    menuAffichage->addAction(actionCacherConnexion);
    QAction *actionAfficherConnexion = new QAction("Affi&cher la partie connexion", this);
    menuAffichage->addAction(actionAfficherConnexion);

    zoneCentrale->setLayout(layout);

    setCentralWidget(zoneCentrale);

    pseudo->setEnabled(false);
    message->setEnabled(false);
    boutonEnvoyer->setEnabled(false);
    boutonEffacer->setEnabled(false);
    textePseudo->setEnabled(false);
    texteMessage->setEnabled(false);
    boutonDeconnexion->setEnabled(false);
    actionDeconnexion->setEnabled(false);

    connect(socket, SIGNAL(readyRead()), this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnecte()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(erreurSocket(QAbstractSocket::SocketError)));
    connect(boutonEffacer, SIGNAL(clicked()), this, SLOT(on_boutonEffacer_clicked()));
    connect(actionEffacer, SIGNAL(triggered()), this, SLOT(on_boutonEffacer_clicked()));
    connect(boutonQuitter, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(actionQuitter, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(boutonAbout, SIGNAL(clicked()), this, SLOT(about()));
    connect(boutonConnexion, SIGNAL(clicked()), this, SLOT(on_boutonConnexion_clicked()));
    connect(actionConnexion, SIGNAL(triggered()), this, SLOT(on_boutonConnexion_clicked()));
    connect(boutonDeconnexion, SIGNAL(clicked()), this, SLOT(on_boutonDeconnexion_clicked()));
    connect(actionDeconnexion, SIGNAL(triggered()), this, SLOT(on_boutonDeconnexion_clicked()));
    connect(boutonSauvegarder, SIGNAL(clicked()), this, SLOT(enregistrer()));
    connect(boutonEnvoyer, SIGNAL(clicked()), this, SLOT(on_boutonEnvoyer_clicked()));
    connect(texteMessage, SIGNAL(returnPressed()), this, SLOT(on_boutonEnvoyer_clicked()));
    connect(actionCacherConnexion, SIGNAL(triggered()), ip, SLOT(hide()));
    connect(actionCacherConnexion, SIGNAL(triggered()), texteIP, SLOT(hide()));
    connect(actionCacherConnexion, SIGNAL(triggered()), port, SLOT(hide()));
    connect(actionCacherConnexion, SIGNAL(triggered()), textePort, SLOT(hide()));
    connect(actionCacherConnexion, SIGNAL(triggered()), boutonConnexion, SLOT(hide()));
    connect(actionCacherConnexion, SIGNAL(triggered()), boutonDeconnexion, SLOT(hide()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), ip, SLOT(show()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), texteIP, SLOT(show()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), port, SLOT(show()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), textePort, SLOT(show()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), boutonConnexion, SLOT(show()));
    connect(actionAfficherConnexion, SIGNAL(triggered()), boutonDeconnexion, SLOT(show()));

    tailleMessage = 0;
}

void FenClient::about()
{
    QMessageBox::information(this, "A propos de RubyChat", "RubyChat est une application créée par Rubytox, dans le cadre de la programmation C++ avec Qt et du partage réseau.");
}

void FenClient::on_boutonEffacer_clicked()
{
    listeMessages->clear();
}

void FenClient::on_boutonConnexion_clicked()
{
    listeMessages->append(tr("<em>Tentative de connexion en cours...</em>"));
    boutonConnexion->setEnabled(false);
    boutonDeconnexion->setEnabled(true);
    message->setEnabled(true);
    boutonEnvoyer->setEnabled(true);
    boutonEffacer->setEnabled(true);
    texteMessage->setEnabled(true);
    actionDeconnexion->setEnabled(true);
    actionConnexion->setEnabled(false);

    socket->abort();
    socket->connectToHost(texteIP->text(), textePort->value());
}

void FenClient::on_boutonDeconnexion_clicked()
{
    boutonConnexion->setEnabled(true);
    boutonDeconnexion->setEnabled(false);
    message->setEnabled(false);
    boutonEnvoyer->setEnabled(false);
    boutonEffacer->setEnabled(false);
    texteMessage->setEnabled(false);
    actionDeconnexion->setEnabled(false);
    actionConnexion->setEnabled(false);

    socket->abort();
}

void FenClient::on_boutonEnvoyer_clicked()
{
    if(!textePseudo->text().isEmpty())
    {
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        QString messageAEnvoyer = tr("<strong>") + textePseudo->text() + tr("</strong> :  ") + texteMessage->text();
        if(texteMessage->text().left(5) == "/say ")
        {
            QString variableFonction = texteMessage->text().right(texteMessage->text().size() - 5);
            listeMessages->append(variableFonction);
            texteMessage->clear();
            return;
        }
        else if(texteMessage->text().left(5) == "/quit")
        {
            boutonQuitter->click();
        }
        else if(texteMessage->text().left(6) == "/nick ")
        {
            QString variableFonction = texteMessage->text().right(texteMessage->text().size() - 6);
            textePseudo->setText(variableFonction);
            texteMessage->clear();
            return;
        }
        else if(texteMessage->text().left(7) == "/logout")
        {
            boutonDeconnexion->click();
            texteMessage->clear();
            return;
        }
        else if(texteMessage->text().left(6) == "/count")
        {
            messageAEnvoyer = texteMessage->text().left(6);
            texteMessage->clear();
        }
        else if(texteMessage->text().left(5) == "/help")
        {
            QString helpTitre = tr("<div style=\"color:green;\"><br />=== LISTE DES COMMANDES ===<br />");
            QString help1 = tr("1. /say &lt;message&gt; => écrit &lt;message&gt;.<br />");
            QString help2 = tr("2. /quit => quitte le chat.<br />");
            QString help3 = tr("3. /nick &lt;pseudo&gt; => Change votre pseudo.<br />");
            QString help4 = tr("4. /logout => Se déconnecter.<br />");
            QString help5 = tr("5. /count => Nombre de clients connectés.<br />");
            QString helpFin = tr("</div>");
            QString messageHelp;
            messageHelp = helpTitre + help1 + help2 + help3 + help4 + help5 + helpFin;
            listeMessages->append(messageHelp);
            texteMessage->clear();
            return;
        }

        out << (quint16) 0;
        out << messageAEnvoyer;
        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet);

        texteMessage->clear();
        texteMessage->setFocus();
    }
    else
    {
        QMessageBox::critical(this, "Erreur", "<strong>ERREUR:</strong> Vous devez choisir un pseudo!");
        texteMessage->clear();
        textePseudo->setFocus();
    }
}

void FenClient::donneesRecues()
{
    QDataStream in(socket);

    if(tailleMessage == 0)
    {
        if(socket->bytesAvailable() < (int)sizeof(quint16))
            return;

        in >> tailleMessage;
    }

    if(socket->bytesAvailable() < tailleMessage)
        return;

    QString messageRecu;
    in >> messageRecu;

    listeMessages->append(messageRecu);

    tailleMessage = 0;
}

void FenClient::connecte()
{
    listeMessages->append(tr("<em>Connexion réussie !</em>"));
    boutonConnexion->setEnabled(false);
    ip->setEnabled(false);
    texteIP->setEnabled(false);
    textePort->setEnabled(false);
    port->setEnabled(false);
    message->setEnabled(true);
    boutonEnvoyer->setEnabled(true);
    boutonEffacer->setEnabled(true);
    texteMessage->setEnabled(true);
    actionDeconnexion->setEnabled(true);
    actionConnexion->setEnabled(false);

    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QString messageAEnvoyer = "pseudo:" + textePseudo->text();

    out << (quint16) 0;
    out << messageAEnvoyer;
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet);
}

void FenClient::deconnecte()
{
    listeMessages->append(tr("<em>Déconnecté du serveur.</em>"));
    boutonConnexion->setEnabled(true);
    ip->setEnabled(true);
    texteIP->setEnabled(true);
    textePort->setEnabled(true);
    port->setEnabled(true);
    message->setEnabled(false);
    boutonEnvoyer->setEnabled(false);
    boutonEffacer->setEnabled(false);
    texteMessage->setEnabled(false);
    actionDeconnexion->setEnabled(false);
    actionConnexion->setEnabled(true);
}

void FenClient::erreurSocket(QAbstractSocket::SocketError erreur)
{
    switch(erreur)
    {
    case QAbstractSocket::HostNotFoundError:
        listeMessages->append(tr("<em><strong>ERREUR:</strong> le serveur n'a pas pu être trouvé. Vérifiez l'IP et le port.</em>"));
    break;

    case QAbstractSocket::ConnectionRefusedError:
        listeMessages->append(tr("<em><strong>ERREUR:</strong> le serveur a refusé la connexion. Vérifiez si le programme \"serveur\" a bien été lancé.</em>"));
    break;

    case QAbstractSocket::RemoteHostClosedError:
        listeMessages->append(tr("<em><strong>ERREUR:</strong> le serveur a coupé la connexion.</em>"));
    break;

    default:
        listeMessages->append(tr("<em><strong>ERREUR:</strong>") + socket->errorString() + tr("</em>"));
    }

    boutonConnexion->setEnabled(true);
}

void FenClient::enregistrer()
{
    QString cheminFichier = QFileDialog::getSaveFileName(this, "Sauvegarder un log", "C:\\Users\\Christophe\\Desktop", "Format texte (*.txt)");

    QFile sauvegardeRTF(cheminFichier);

    listeMessages->toPlainText().replace("\n", "\n\r");
    if(sauvegardeRTF.open(QFile::WriteOnly))
    {
      QTextStream out(&sauvegardeRTF);
      out << listeMessages->toPlainText();
    }
}
