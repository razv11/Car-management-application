#pragma once

#include "../Controller/serviceMasina.h"
#include "../Domain/Masina.h"
#include "./ListModel.h"

#include <string>
using std::string;

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qlabel.h>

class deleteWindow : public QWidget {
private:
	serviceMasina& service;

	QListWidget* listaMasini = new QListWidget;

	QPushButton* btnDelete = new QPushButton{ "&Sterge" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	QLabel* lblTotal = new QLabel;

	QListView* list = new QListView;
	MyListModel* model;

	void initDeleteWindow() {
		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		mainLayout->addWidget(lblTotal);
		lblTotal->setText(QString::fromStdString("Total masini: " + std::to_string((int)service.getAll().size())));

		list->setUniformItemSizes(true);
		model = new MyListModel{ service.getAll() };
		list->setModel(model);
		//mainLayout->addWidget(listaMasini);
		mainLayout->addWidget(list);

		QHBoxLayout* buttonsLayout = new QHBoxLayout;
		buttonsLayout->addWidget(btnDelete);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnExit);
		mainLayout->addLayout(buttonsLayout);
	}

	void reloadData() {
		listaMasini->clear();
		for (auto const& masina : service.getAll()) {
			string details = masina.getNrInmatriculare() + "  |  " + masina.getProducator() + "  |  " + masina.getModel() + "  |  " + masina.getTip();
			listaMasini->addItem(QString::fromStdString(details));
		}
	}

	void connectSignalsSlots() {
		QObject::connect(btnDelete, &QPushButton::clicked, [&]() {
			/*
			auto selectedItem = listaMasini->selectedItems();
			if (!selectedItem.isEmpty()) {

				auto details = selectedItem.at(0)->text().toStdString();
				int i = 0;
				for (i = 0; i < details.length(); i++) {
					if (details[i] == ' ') {
						break;
					}
				}
				auto nrInmatriculare = details.substr(0, i);

				QMessageBox::StandardButton reply;
				reply = QMessageBox::question(this, "Delete", "Sigur doriti sa stergeti masina?", QMessageBox::Yes | QMessageBox::No);
				if (reply == QMessageBox::Yes) {
					service.StergeMasinaService(nrInmatriculare);
					reloadData();
					QMessageBox::information(nullptr, "Info", "Masina a fost stearsa cu succes!");

					lblTotal->setText(QString::fromStdString("Total masini: " + std::to_string((int)service.getAll().size())));
				
					model->setMasini(service.getAll());
				}
			}
			*/

			if (list->selectionModel()->selectedIndexes().isEmpty()) {
				return;
			}

			auto selIndex = list->selectionModel()->selectedIndexes().at(0);
			QString nrInmatrciulare = selIndex.data(Qt::DisplayRole).toString();

			service.StergeMasinaService(nrInmatrciulare.toStdString());
			model->setMasini(service.getAll());
			lblTotal->setText(QString::fromStdString("Total masini: " + std::to_string((int)service.getAll().size())));

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
	deleteWindow(serviceMasina& serv) : service{ serv } {
		initDeleteWindow();
		connectSignalsSlots();
		//reloadData();
	}
};