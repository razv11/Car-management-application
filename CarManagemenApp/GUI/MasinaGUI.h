#pragma once

#include "../Controller/serviceMasina.h"
#include "../Domain/Masina.h"
#include "./addWindow.h"
#include "./deleteWindow.h"
#include "./modifyWindow.h"
#include "./showWindow.h"
#include "./filterWindow.h"
#include "./carwashWindow.h"
#include "./searchWindow.h"
#include "./drawWindow.h"

#include <string>
#include <vector>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qmessagebox.h>

class MasinaGUI : public QWidget {

private:
    serviceMasina& service;

    QPushButton* btnAdauga = new QPushButton{ "&Adauga" };
    QPushButton* btnSterge = new QPushButton{ "&Sterge" };
    QPushButton* btnModifica = new QPushButton{ "&Modifica" };
    QPushButton* btnShow = new QPushButton{ "&Vizualizare/sortare" };
    QPushButton* btnFiltru = new QPushButton{ "&Filtreaza" };
    QPushButton* btnSpalatorie = new QPushButton{ "&Spalatorie" };
    QPushButton* btnExit = new QPushButton{ "&Exit" };
    QPushButton* btnSearch = new QPushButton{ "&Cauta" };
    QPushButton* btnUndo = new QPushButton{ "&Undo" };

    QPushButton* btnGenerateButtons = new QPushButton{ "&Genereaza butoane" };
    QPushButton* btnDraw = new QPushButton{ "&Draw" };
    QLabel* generateButtonsLabel = new QLabel;

    void initGUI();

    void initConnect();

public:
    MasinaGUI(serviceMasina& service) : service{ service } {
		initGUI();  
        initConnect();
	}
};

