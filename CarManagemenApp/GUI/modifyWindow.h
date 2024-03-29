#pragma once

#include "../Controller/serviceMasina.h"
#include "./ListModel.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>

class modifyWindow : public QWidget {
private:
	serviceMasina& service;

	QLineEdit* nrInmatriculare = new QLineEdit;
	QLineEdit* producator = new QLineEdit;
	QLineEdit* model = new QLineEdit;
	QLineEdit* tip = new QLineEdit;

	QListWidget* listaMasini = new QListWidget;
	QPushButton* btnModify = new QPushButton{ "&Modifica" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	QListView* list = new QListView;
	MyListModel* modelList;

	void initModifyWindow() {
		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		QHBoxLayout* formAndListLayout = new QHBoxLayout;

		QFormLayout* form = new QFormLayout;
		form->addRow("Numar de inmatriculare: ", nrInmatriculare);
		nrInmatriculare->setDisabled(true);
		form->addRow("Noul producator: ", producator);
		form->addRow("Noul model: ", model);
		form->addRow("Noul tip: ", tip);

		formAndListLayout->addLayout(form);

		list->setUniformItemSizes(true);
		modelList = new MyListModel{ service.getAll() };
		list->setModel(modelList);
		//mainLayout->addWidget(listaMasini);
		formAndListLayout->addWidget(list);

		mainLayout->addLayout(formAndListLayout);

		QHBoxLayout* buttonsLayout = new QHBoxLayout;
		buttonsLayout->addWidget(btnModify);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnExit);
		mainLayout->addLayout(buttonsLayout);
	}

	void connectSignalsSlots() {
		/*
		QObject::connect(listaMasini, &QListWidget::itemSelectionChanged, [&]() {
			auto selectedItem = listaMasini->selectedItems();
			if (selectedItem.isEmpty()) {
				nrInmatriculare->setText("");
				producator->setText("");
				model->setText("");
				tip->setText("");
			}
			else {
				auto nr = selectedItem.at(0);
				nrInmatriculare->setText(nr->text());

				//auto masina = service.CautaMasinaService(nr->text().toStdString());
				//producator->setText(QString::fromStdString(masina.getProducator()));
				//model->setText(QString::fromStdString(masina.getModel()));
				//tip->setText(QString::fromStdString(masina.getTip()));
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

		QObject::connect(btnModify, &QPushButton::clicked, [&]() {
			auto nr = nrInmatriculare->text();
			auto p = producator->text();
			auto m = model->text();
			auto t = tip->text();

			if (!nr.isEmpty()) {
				try {
					service.ModificaMasinaService(nr.toStdString(), p.toStdString(), m.toStdString(), t.toStdString());
					//reloadData();
					QMessageBox::information(nullptr, "Info", "Masina a fost modificata cu succes!");
				}
				catch (validatorMasinaException& validationError) {
					QMessageBox::warning(nullptr, "Warning", QString::fromStdString(validationError.errors));
				}
			}
			else {
				QMessageBox::warning(nullptr, "Warning", "Selectati o masina din lista!");
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

	/*
	void reloadData() {
		listaMasini->clear();
		for (const auto& masina : service.getAll()) {
			listaMasini->addItem(QString::fromStdString(masina.getNrInmatriculare()));
		}
	}
	*/

public:
	modifyWindow(serviceMasina& serv) : service{ serv } {
		initModifyWindow();
		connectSignalsSlots();
		//reloadData();
	}
};