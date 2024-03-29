#include <QtWidgets/QApplication>
#include "./GUI/MasinaGUI.h"

#include "./Validation/validatorMasina.h"
#include "./Repo/repoMasina.h"
#include "./Repo/repoSpalatorie.h"
#include "./Controller/serviceMasina.h"

void initializare(serviceMasina& serv) {
    serv.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
    serv.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");
    serv.AdaugaMasinaService("SM95SVP", "Tesla", "S", "sport");
    serv.AdaugaMasinaService("CJ01AAA", "Audi", "A5", "sportback");
    serv.AdaugaMasinaService("BV99ZZZ", "Audi", "A7", "sport");
    serv.AdaugaMasinaService("IS15SUN", "BMW", "Seria 5", "sport");
    serv.AdaugaMasinaService("CJ55ABC", "Skoda", "Octavia", "berlina");
    serv.AdaugaMasinaService("AB12OOP", "Renault", "Laguna", "break");
}

void testAll() {
    //testRepo();
    //testService();
    testValidate();
    testRepoSpalatorie();
}


int main(int argc, char *argv[])
{
    testAll();
    
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon("icon.jpg"));

    repoMasina repo{ "masini.txt" };
    repoSpalatorie spalatorie;
    validatorMasina validator;
    serviceMasina serv{ repo, validator, spalatorie };
    initializare(serv);

    MasinaGUI GUI{ serv };
    GUI.setWindowTitle("Gestiune Masini");
    GUI.show();

    return app.exec();
}
