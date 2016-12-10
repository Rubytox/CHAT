#include <QApplication>
#include "FenServeur.h"

int main(int argc, char* argv[])
{
    int currentExitCode = 0;

    do
    {
        QApplication app(argc, argv);

        FenServeur fenetre;
        fenetre.show();
        currentExitCode = app.exec();

    }while(currentExitCode == FenServeur::EXIT_CODE_REBOOT);

    return currentExitCode;
}
