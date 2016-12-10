#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include <QtNetwork>

class FenClient;

class Client : public QWidget
{
    Q_OBJECT

public:
    Client(QTcpSocket socket, QString pseudonyme, quint16 taille);

    QString getPseudo() const;
    quint16 getTailleMessage() const;
    QTcpSocket getSocket() const;
    ~Client();
public slots:
    void setPseudo(QString);
    void setTailleMessage(quint16);
    void setSocket(QTcpSocket *);
private :
    QTcpSocket m_socketClient;
    QString m_pseudo;
    quint16 m_tailleMessage;

};

#endif // CLIENT_H
