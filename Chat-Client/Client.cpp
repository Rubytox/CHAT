#include "FenClient.h"
#include "Client.h"

Client::Client(QTcpSocket socket, QString nom, quint16 taille) : m_socketClient(socket), m_pseudo(nom), m_tailleMessage(taille)
{
}

QString Client::getPseudo() const
{
    return m_pseudo;
}

quint16 Client::getTailleMessage() const
{
    return m_tailleMessage;
}

QTcpSocket Client::getSocket() const
{
    return m_socketClient;
}

/*void Client::setPseudo(QString nom)
{
    m_pseudo = nom;
}

void Client::setTailleMessage(quint16 taille)
{
    m_tailleMessage = taille;
}

void Client::setSocket(QTcpSocket *socket)
{
    m_socketClient = socket;
}*/

Client::~Client()
{

}
