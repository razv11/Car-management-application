#include "MasinaGUI.h"

void MasinaGUI::initGUI() {
    setStyleSheet("background-color: light gray");
    setMaximumHeight(0);
    setMaximumWidth(0);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);

    QHBoxLayout* firstSetButtons = new QHBoxLayout;
    btnAdauga->setStyleSheet("color: white; background-color: gray");
    firstSetButtons->addWidget(btnAdauga); 
    btnSterge->setStyleSheet("color: white; background-color: gray");
    firstSetButtons->addWidget(btnSterge);
    btnModifica->setStyleSheet("color: white; background-color: gray");
    firstSetButtons->addWidget(btnModifica);
    mainLayout->addLayout(firstSetButtons);


    QLabel* label = new QLabel;
    label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    label->setMinimumHeight(250);
    label->setMinimumWidth(450);
    label->setStyleSheet("background-image: url(./GUI/cars.jpg); color: black; background-color: cyan; font-size: 25px");
    mainLayout->addWidget(label);

    QHBoxLayout* secondSetButtons = new QHBoxLayout;
    btnFiltru->setStyleSheet("color: white; background-color: grey");
    secondSetButtons->addWidget(btnFiltru);
    btnShow->setStyleSheet("color: white; background-color: grey");
    secondSetButtons->addWidget(btnShow);
    btnSpalatorie->setStyleSheet("color: white; background-color: grey");
    secondSetButtons->addWidget(btnSpalatorie);
    mainLayout->addLayout(secondSetButtons);

    QHBoxLayout* thirdSetButtons = new QHBoxLayout;
    thirdSetButtons->addStretch();
    btnSearch->setStyleSheet("color: white; background-color: grey");
    thirdSetButtons->addWidget(btnSearch);
    btnUndo->setStyleSheet("color: white; background-color: grey");
    thirdSetButtons->addWidget(btnUndo);
    thirdSetButtons->addStretch();
    mainLayout->addLayout(thirdSetButtons);

    QHBoxLayout* generateButtonsLayout = new QHBoxLayout;
    generateButtonsLayout->addStretch();
    btnGenerateButtons->setStyleSheet("color: white; background-color: grey");
    generateButtonsLayout->addWidget(btnGenerateButtons);
    generateButtonsLayout->addStretch();
    mainLayout->addLayout(generateButtonsLayout);

    mainLayout->addSpacing(10);

    QHBoxLayout* drawLayout = new QHBoxLayout;
    drawLayout->addStretch();
    btnDraw->setStyleSheet("color: white; background-color: grey");
    drawLayout->addWidget(btnDraw);
    drawLayout->addStretch();
    mainLayout->addLayout(drawLayout);

    QHBoxLayout* exitButtonLayout = new QHBoxLayout;
    exitButtonLayout->addStretch();
    btnExit->setStyleSheet("color: white; background-color: black");
    exitButtonLayout->addWidget(btnExit);
    exitButtonLayout->addStretch();
    mainLayout->addLayout(exitButtonLayout);
}

void MasinaGUI::initConnect() {
    QObject::connect(btnExit, &QPushButton::clicked, [&]() {

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Exit", "Sigur doriti sa iesiti?", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            close();
        }
        });

    QObject::connect(btnAdauga, &QPushButton::clicked, [&]() {
        auto ADDWindow = new addWindow{ service };
        ADDWindow->setWindowModality(Qt::ApplicationModal);
        ADDWindow->show();

    });

    QObject::connect(btnSterge, &QPushButton::clicked, [&]() {
        auto DELETEWindow = new deleteWindow{ service };
        DELETEWindow->setWindowModality(Qt::ApplicationModal);
        DELETEWindow->show();

        });

    QObject::connect(btnModifica, &QPushButton::clicked, [&]() {
        auto MODIFYWindow = new modifyWindow{ service };
        MODIFYWindow->setWindowModality(Qt::ApplicationModal);
        MODIFYWindow->show();

        });

    QObject::connect(btnFiltru, &QPushButton::clicked, [&]() {
        auto FILTERWindow = new filterWindow{ service };
        FILTERWindow->setWindowModality(Qt::ApplicationModal);
        FILTERWindow->show();
        
        });

    QObject::connect(btnShow, &QPushButton::clicked, [&]() {
        auto SHOWWindow = new showWindow{ service };
        SHOWWindow->setWindowModality(Qt::ApplicationModal);
        SHOWWindow->setMinimumWidth(600);
        SHOWWindow->show();
        
        });

    QObject::connect(btnSpalatorie, &QPushButton::clicked, [&]() {
        auto CARWASHWindow = new carWashWindow{ service };
        CARWASHWindow->setWindowModality(Qt::ApplicationModal);
        CARWASHWindow->show();
        
        });

    QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
        auto SEARCHWindow = new searchWindow{ service };
        SEARCHWindow->setWindowModality(Qt::ApplicationModal);
        SEARCHWindow->show();
        
        });

    QObject::connect(btnUndo, &QPushButton::clicked, [&]() {
        try {
            service.undoService();
            QMessageBox::information(nullptr, "Info", "Undo realizat cu succes!");
        }
        catch (const char* error) {
            QMessageBox::warning(nullptr, "Warning", QString::fromStdString(error));
        }

        });

    QObject::connect(btnDraw, &QPushButton::clicked, [&]() {
        auto DRAWWindow = new drawWindow{ service };
        
        DRAWWindow->show();

        });

    QObject::connect(btnGenerateButtons, &QPushButton::clicked, [&]() {
        generateButtonsLabel->clear();
        generateButtonsLabel->setWindowTitle(" ");
        generateButtonsLabel->setMinimumWidth(160);
        generateButtonsLabel->setWindowModality(Qt::ApplicationModal);

        std::map<std::string, int> types;
        QVBoxLayout* generareLayout = new QVBoxLayout;
        for (const auto& masina : service.getAll()) {
            auto tip = masina.getTip();
            types[tip]++;
            if (types[masina.getTip()] <= 1) {
               auto btn = new QPushButton{ QString::fromStdString(masina.getTip()) };
               generareLayout->addWidget(btn);

               QObject::connect(btn, &QPushButton::clicked, [this, btn, masina, tip]() {
                    int contor = 0;
                    for (const auto& masina : service.getAll()) {
                        if (masina.getTip() == tip) {
                              contor++;
                        }
                    }
                    auto num_str = std::to_string(contor);
                    std::string stringSTD;
                    if (num_str == "1") {
                        stringSTD = "Exista o masina de tipul " + tip;
                    }
                    else {
                        stringSTD = "Exista " + num_str + " masini de tipul " + tip;
                    }
                    QMessageBox::information(nullptr, "Info", QString::fromStdString(stringSTD));
               });
            }
        }
        generateButtonsLabel->setLayout(generareLayout);
        generateButtonsLabel->show();

        });
}