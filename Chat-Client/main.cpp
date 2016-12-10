#include <QApplication>
#include "FenClient.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QString locale = QLocale::system().name().section('_', 0, 0);
    QTranslator translator;
    translator.load(QString("rubychat_") + locale);
    app.installTranslator(&translator);

    FenClient fenetre;
    fenetre.show();

    return app.exec();
}
