#pragma once

#include "../Controller/serviceMasina.h"
#include "../Observer/observer.h"
#include "./ListModel.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>

class carWashWindow : public QWidget, public Observer {
private:
	serviceMasina& service;

	QListWidget* carList = new QListWidget;
	QListWidget* carWashList = new QListWidget;

	QLineEdit* nrInmatriculare = new QLineEdit;

	QPushButton* btnAdauga = new QPushButton{ "&Add" };
	QPushButton* btnGolesteLista = new QPushButton{ "&Clear list" };
	QPushButton* btnGenereazaLista = new QPushButton{"&Random list"};
	QPushButton* btnExportFile = new QPushButton{ "&Export" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	QLabel* genereazaLabel = new QLabel;
	QLineEdit* inputNumber = new QLineEdit;
	QPushButton* btnOkGenereaza = new QPushButton{ "&Ok" };
	QPushButton* btnCancelGenereaza = new QPushButton{ "&Cancel" };

	QLabel* exportLabel = new QLabel;
	QLineEdit* fileName = new QLineEdit;
	QPushButton* btnOkExport = new QPushButton{ "&Ok" };
	QPushButton* btnCancelExport = new QPushButton{ "&Cancel" };

	QListView* list = new QListView;
	MyListModel* modelList;

	QListView* washList = new QListView;
	MyListModel* modelWashList;

	void initCarWashWindow() {
		service.addObserver(this);

		setWindowTitle("Car wash");

		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		QHBoxLayout* listsLayout = new QHBoxLayout;
		
		{
			QFormLayout* form = new QFormLayout;
			nrInmatriculare->setDisabled(true);
			form->addRow("Numar inmatriculare", nrInmatriculare);

			QHBoxLayout* leftButtonsLayout = new QHBoxLayout;
			leftButtonsLayout->addWidget(btnAdauga);
			leftButtonsLayout->addWidget(btnGolesteLista);
			leftButtonsLayout->addWidget(btnGenereazaLista);

			QVBoxLayout* leftSide = new QVBoxLayout;
			QLabel* carsLabel = new QLabel{ "Lista masini" };
			carsLabel->setAlignment(Qt::AlignHCenter);
			carsLabel->setStyleSheet("font-size: 18px; color: blue");
			leftSide->addWidget(carsLabel);


			list->setUniformItemSizes(true);
			modelList = new MyListModel{ service.getAll() };
			list->setModel(modelList);
			//leftSide->addWidget(carList);
			leftSide->addWidget(list);


			leftSide->addSpacing(20);
			leftSide->addLayout(form);
			leftSide->addSpacing(20);
			leftSide->addLayout(leftButtonsLayout);
			leftSide->addSpacing(10);
			listsLayout->addLayout(leftSide);
		}

		{
			QHBoxLayout* rightSideButtonsLayout = new QHBoxLayout;
			{
				rightSideButtonsLayout->addStretch();

				QHBoxLayout* buttonsLayout = new QHBoxLayout;
				buttonsLayout->addWidget(btnExportFile);
				buttonsLayout->addWidget(btnExit);

				rightSideButtonsLayout->addLayout(buttonsLayout);
				rightSideButtonsLayout->addStretch();
			}

			QVBoxLayout* rightSide = new QVBoxLayout;
			QLabel* carWashLabel = new QLabel{ "Masini spalatorie" };
			carWashLabel->setAlignment(Qt::AlignHCenter);
			carWashLabel->setStyleSheet("font-size: 18px; color: blue");
			rightSide->addWidget(carWashLabel);


			washList->setUniformItemSizes(true);
			modelWashList = new MyListModel{ service.getAllSpalatorie() };
			washList->setModel(modelWashList);
			rightSide->addWidget(washList);
			//rightSide->addWidget(carWashList);


			rightSide->addSpacing(20);
			rightSide->addLayout(rightSideButtonsLayout);
			rightSide->addSpacing(10);
			listsLayout->addLayout(rightSide);
		}
		
		mainLayout->addLayout(listsLayout);
	}

	void connectSignalsSlots() {
		QObject::connect(btnAdauga, &QPushButton::clicked, [&]() {
			auto nr = nrInmatriculare->text();
			if (!nr.isEmpty()) {
				try {
					service.AdaugaMasinaSpalatorieService(nr.toStdString());
					//loadCarWashList();
					modelWashList->setMasini(service.getAllSpalatorie());
					QMessageBox::information(nullptr, "Info", "Masina a fost adaugata cu succes in spalatorie!");
				}
				catch (repoMasinaException& repoError) {
					QMessageBox::warning(nullptr, "Warning", QString::fromStdString(repoError.errors));
				}
				catch (const char* error) {
					QMessageBox::warning(nullptr, "Warning", QString::fromStdString(error));
				}
			}
			else {
				QMessageBox::warning(nullptr, "Warning", "Selectati o masina din lista!");
			}
			
			});

		QObject::connect(btnGolesteLista, &QPushButton::clicked, [&]() {
			try {
				service.GolesteMasiniSpalatorieService();
				carWashList->clear();
			}
			catch (const char* error) {
				QMessageBox::information(nullptr, "Info", QString::fromStdString(error));
			}
				
			
			});

		QObject::connect(btnGenereazaLista, &QPushButton::clicked, [&]() {
			genereazaLabel->setMinimumWidth(350);
			genereazaLabel->setWindowModality(Qt::ApplicationModal);
			genereazaLabel->setWindowTitle("Random list");

			QVBoxLayout* inputNumberLayout = new QVBoxLayout;

			QFormLayout* form = new QFormLayout;
			inputNumber->setText("");
			form->addRow("Numarul de masini de generat", inputNumber);
			inputNumberLayout->addLayout(form);
			inputNumberLayout->addStretch();

			QHBoxLayout* buttons = new QHBoxLayout;
			buttons->addStretch();
			buttons->addWidget(btnOkGenereaza);
			buttons->addWidget(btnCancelGenereaza);
			inputNumberLayout->addLayout(buttons);

			genereazaLabel->setLayout(inputNumberLayout);
			genereazaLabel->show();

			});

		QObject::connect(btnOkGenereaza, &QPushButton::clicked, [&]() {
			auto numberToGenerate = inputNumber->text();
			try {
				service.GenereazaAleatorMasiniSpalatorieService(numberToGenerate.toStdString());
				//loadCarWashList();
				modelWashList->setMasini(service.getAllSpalatorie());
				QMessageBox::information(nullptr, "Info", "Masinile au fost generate cu succes!");
				genereazaLabel->close();
			}
			catch (const char* error) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(error));
			}

			});

		QObject::connect(btnCancelGenereaza, &QPushButton::clicked, [&]() {
			genereazaLabel->close();

			});

		QObject::connect(btnExportFile, &QPushButton::clicked, [&]() {
			exportLabel->setMinimumWidth(350);
			exportLabel->setWindowModality(Qt::ApplicationModal);
			exportLabel->setWindowTitle("Export cars");

			QVBoxLayout* inputFileLayout = new QVBoxLayout;

			QFormLayout* formFile = new QFormLayout;
			fileName->setText("");
			formFile->addRow("Nume fisier", fileName);
			inputFileLayout->addLayout(formFile);
			inputFileLayout->addStretch();

			QHBoxLayout* buttonsFileLayout = new QHBoxLayout;
			buttonsFileLayout->addStretch();
			buttonsFileLayout->addWidget(btnOkExport);
			buttonsFileLayout->addWidget(btnCancelExport);
			inputFileLayout->addLayout(buttonsFileLayout);

			exportLabel->setLayout(inputFileLayout);
			exportLabel->show();

			});

		QObject::connect(btnOkExport, &QPushButton::clicked, [&]() {
			auto fisier = fileName->text();
			service.ExportaMasiniSpalatorieService(fisier.toStdString());
			QMessageBox::information(nullptr, "Info", "Masinile au fost exportat cu succes!");
			exportLabel->close();

			});

		QObject::connect(btnCancelExport, &QPushButton::clicked, [&]() {
			exportLabel->close();

			});

		/*
		QObject::connect(carList, &QListWidget::itemSelectionChanged, [&]() {
			auto selectedItem = carList->selectedItems();
			if (selectedItem.isEmpty()) {
				nrInmatriculare->setText("");
			}
			else {
				auto details = selectedItem.at(0)->text().toStdString();
				int i = 0;
				for (i = 0; i < details.length(); i++) {
					if (details[i] == ' ') {
						break;
					}
				}
				auto nr = details.substr(0, i);
				nrInmatriculare->setText(QString::fromStdString(nr));
			}
			
			});
		*/

		QObject::connect(list->selectionModel(), &QItemSelectionModel::selectionChanged, [&]() {
			if (list->selectionModel()->selectedIndexes().isEmpty()) {
				//nu este nimic selectat (golesc detaliile)
				return;
			}

			auto selIndex = list->selectionModel()->selectedIndexes().at(0);
			QString nr = selIndex.data(Qt::DisplayRole).toString();

			nrInmatriculare->setText(nr);

			});

		QObject::connect(btnExit, &QPushButton::clicked, [&]() {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(this, "Exit", "Sigur doriti sa inchideti fereastra?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				close();
			}

			});
	}

	/*
	void loadCarList() {
		carList->clear();
		for (const auto& masina : service.getAll()) {
			string details = masina.getNrInmatriculare() + "  -  " + masina.getProducator() + "  -  " + masina.getModel() + "  -  " + masina.getTip();
			carList->addItem(QString::fromStdString(details));
		}
	}

	void loadCarWashList() {
		carWashList->clear();
		for (const auto& masina : service.getAllSpalatorie()) {
			string details = masina.getNrInmatriculare() + "  -  " + masina.getProducator() + "  -  " + masina.getModel() + "  -  " + masina.getTip();
			carWashList->addItem(QString::fromStdString(details));
		}
	}
	*/

	void update() override {
		modelList->setMasini(service.getAll());
		modelWashList->setMasini(service.getAllSpalatorie());
		//loadCarList();
		//loadCarWashList();
	}

public:
	carWashWindow(serviceMasina& serv) : service{ serv } {
		initCarWashWindow();
		connectSignalsSlots();
		//loadCarList();
		//loadCarWashList();
	}

};