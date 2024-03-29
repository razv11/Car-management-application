#pragma once

#include "../Controller/serviceMasina.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qdialogbuttonbox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qformlayout.h>

class filterWindow : public QWidget {
private:
	serviceMasina& service;

	QListWidget* listaMasini = new QListWidget;

	QPushButton* btnFilterType = new QPushButton{ "&Filter by type" };
	QPushButton* btnFilterManufacturer = new QPushButton{ "&Filter by manufacturer" };
	QPushButton* btnClear = new QPushButton{ "&Clear list" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	QLabel* filterTypeLabel = new QLabel;
	QLabel* filterManufacturerLabel = new QLabel;

	QLineEdit* filterTypeLine = new QLineEdit;
	QLineEdit* filterManufacturerLine = new QLineEdit;
	QPushButton* btnOkType = new QPushButton("&Ok");
	QPushButton* btnOkManufacturer = new QPushButton("&Ok");
	QPushButton* btnTypeCancel = new QPushButton("&Cancel");
	QPushButton* btnManufacturerCancel = new QPushButton("&Cancel");

	void initFilterWindow() {
		setWindowTitle("Filter cars");

		QHBoxLayout* mainLayout = new QHBoxLayout;
		setLayout(mainLayout);

		QVBoxLayout* buttonsLayout = new QVBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnFilterType);
		buttonsLayout->addWidget(btnFilterManufacturer);
		buttonsLayout->addWidget(btnClear);
		buttonsLayout->addWidget(btnExit);
		buttonsLayout->addStretch();
		mainLayout->addLayout(buttonsLayout);

		mainLayout->addWidget(listaMasini);
	}

	void connectSignalsSlots() {
		QObject::connect(btnFilterType, &QPushButton::clicked, [&]() {
			filterTypeLabel->setMinimumWidth(350);
			filterTypeLabel->setWindowModality(Qt::ApplicationModal);
			filterTypeLabel->setWindowTitle("Filter by type");

			QVBoxLayout* filterTypeLayout = new QVBoxLayout;

			QFormLayout* form = new QFormLayout;
			filterTypeLine->setText("");
			form->addRow("Tip: ", filterTypeLine);
			filterTypeLayout->addLayout(form);
			filterTypeLayout->addStretch();

			QHBoxLayout* buttons = new QHBoxLayout;
			buttons->addStretch();
			buttons->addWidget(btnOkType);
			buttons->addWidget(btnTypeCancel);
			filterTypeLayout->addLayout(buttons);
			filterTypeLabel->setLayout(filterTypeLayout);

			filterTypeLabel->show();

			});

		QObject::connect(btnFilterManufacturer, &QPushButton::clicked, [&]() {
			filterManufacturerLabel->setMinimumWidth(350);
			filterManufacturerLabel->setWindowModality(Qt::ApplicationModal);
			filterManufacturerLabel->setWindowTitle("Filter by type");

			QVBoxLayout* filterManufacturerLayout = new QVBoxLayout;

			QFormLayout* form = new QFormLayout;
			filterManufacturerLine->setText("");
			form->addRow("Manufacturer: ", filterManufacturerLine);
			filterManufacturerLayout->addLayout(form);
			filterManufacturerLayout->addStretch();

			QHBoxLayout* buttons = new QHBoxLayout;
			buttons->addStretch();
			buttons->addWidget(btnOkManufacturer);
			buttons->addWidget(btnManufacturerCancel);
			filterManufacturerLayout->addLayout(buttons);
			filterManufacturerLabel->setLayout(filterManufacturerLayout);

			filterManufacturerLabel->show();
			
			});

		QObject::connect(btnOkType, &QPushButton::clicked, [&]() {
			auto type = filterTypeLine->text();
			if (service.filtreazaTipService(type.toStdString()).size() == 0) {
				QMessageBox::warning(nullptr, "Warning", "Nu exista masini cu tipul specificat!");
			}
			else {
				reloadData(service.filtreazaTipService(type.toStdString()));
				filterTypeLabel->close();
			}
			
			});

		QObject::connect(btnOkManufacturer, &QPushButton::clicked, [&]() {
			auto manufacturer = filterManufacturerLine->text();
			if (service.filtreazaProducatorService(manufacturer.toStdString()).size() == 0) {
				QMessageBox::warning(nullptr, "Warning", "Nu exista masini cu producatorul specificat!");
			}
			else {
				reloadData(service.filtreazaProducatorService(manufacturer.toStdString()));
				filterManufacturerLabel->close();
			}

			});

		QObject::connect(btnTypeCancel, &QPushButton::clicked, [&]() {
			filterTypeLabel->close();

			});

		QObject::connect(btnManufacturerCancel, &QPushButton::clicked, [&]() {
			filterManufacturerLabel->close();

			});

		QObject::connect(btnExit, &QPushButton::clicked, [&]() {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Exit", "Sigur doriti sa inchideti fereastra?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				close();
			}

			});

		QObject::connect(btnClear, &QPushButton::clicked, [&]() {
			listaMasini->clear();

			});
	}

	void reloadData(vector<Masina> masini) {
		listaMasini->clear();
		for (const auto& masina : masini) {
			string details = masina.getNrInmatriculare() + "  |  " + masina.getProducator() + "  |  " + masina.getModel() + "  |  " + masina.getTip();
			listaMasini->addItem(QString::fromStdString(details));
		}
	}

public:
	filterWindow(serviceMasina& serv) : service{ serv } {
		initFilterWindow();
		connectSignalsSlots();
	}
};