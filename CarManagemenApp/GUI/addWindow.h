#pragma once

#include "../Controller/serviceMasina.h"
#include "../Domain/Masina.h"

#include <string>
using std::string;

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>

class addWindow : public QWidget {
private:
	serviceMasina& service;

	QLineEdit* nrInmatriculare = new QLineEdit;
	QLineEdit* producator = new QLineEdit;
	QLineEdit* model = new QLineEdit;
	QLineEdit* tip = new QLineEdit;

	QPushButton* btnAdauga = new QPushButton{ "&Adauga" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	void initAddWindow() {
		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		QFormLayout* form = new QFormLayout;
		nrInmatriculare->setStyleSheet("color: blue; background-color: cyan");
		form->addRow("Numar inmatriculare: ", nrInmatriculare);
		producator->setStyleSheet("color: blue; background-color: cyan");
		form->addRow("Producator: ", producator);
		model->setStyleSheet("color: blue; background-color: cyan");
		form->addRow("Model: ", model);
		tip->setStyleSheet("color: blue; background-color: cyan");
		form->addRow("Tip: ", tip);
		mainLayout->addLayout(form);

		QHBoxLayout* buttonsLayout = new QHBoxLayout;
		buttonsLayout->addWidget(btnAdauga);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnExit);
		mainLayout->addLayout(buttonsLayout);
	}

	void connectSignalsSlots() {
		QObject::connect(btnAdauga, &QPushButton::clicked, [&]() {
			auto nr = nrInmatriculare->text();
			auto p = producator->text();
			auto m = model->text();
			auto t = tip->text();

			try {
				service.AdaugaMasinaService(nr.toStdString(), p.toStdString(), m.toStdString(), t.toStdString());
				QMessageBox::information(nullptr, "Info", "Masina a fost adaugata cu succes!");
			}
			catch (validatorMasinaException& validationError) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(validationError.errors));
			}
			catch (repoMasinaException& repoError) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(repoError.errors));
			}

			});

		QObject::connect(btnExit, &QPushButton::clicked, [&]() {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Exit", "Sigur doriti sa inchideti fereastra?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				close();
			}

			});
	}

public:
	addWindow(serviceMasina& serv) : service{ serv } {
		initAddWindow();
		connectSignalsSlots();
	}
};