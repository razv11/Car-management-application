#pragma once

#include "../Controller/serviceMasina.h"
#include "../Domain/Masina.h"
#include "./TabelModel.h"

#include <vector>
#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qtablewidget.h>
#include <QtWidgets/qabstractitemview.h>
#include <QtWidgets/qlistwidget.h>
#include <QtWidgets/qheaderview.h>
#include <QtWidgets/qtableview.h>

class showWindow : public QWidget {
private:
	serviceMasina& service;

	QTableWidget* tabel = new QTableWidget{ (int)service.getAll().size(),4};

	QPushButton* btnSortNrInmatriculare = new QPushButton{ "&Sort by number" };
	QPushButton* btnSortTip = new QPushButton{ "&Sort by type" };
	QPushButton* btnSortProducatorTip = new QPushButton{ "&Sort by manufacturer and model" };
	QPushButton* btnExit = new QPushButton{ "&Exit" };

	QTableView* tableView = new QTableView();
	TabelModel* model;

	void initShowWindow() {
		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		QHBoxLayout* buttonsLayout = new QHBoxLayout;
		buttonsLayout->addWidget(btnSortNrInmatriculare);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnSortTip);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnSortProducatorTip);
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnExit);
		mainLayout->addLayout(buttonsLayout);

		tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		model = new TabelModel{ service.getAll() };
		tableView->setModel(model);
		
		//mainLayout->addWidget(tabel);
		mainLayout->addWidget(tableView);
	}

	void connectSignalsSlots() {
		QObject::connect(btnSortNrInmatriculare, &QPushButton::clicked, [&]() {
			reloadData(service.sorteazaNrInmatriculareService());

			});

		QObject::connect(btnSortTip, &QPushButton::clicked, [&]() {
			reloadData(service.sorteazaTipService());

			});

		QObject::connect(btnSortProducatorTip, &QPushButton::clicked, [&]() {
			reloadData(service.sorteazaProducatorAndModelService());

			});

		QObject::connect(btnExit, &QPushButton::clicked, [&]() {
			QMessageBox::StandardButton reply;
			reply = QMessageBox::question(nullptr, "Exit", "Sigur doriti sa inchideti fereastra?", QMessageBox::Yes | QMessageBox::No);
			if (reply == QMessageBox::Yes) {
				close();
			}
			
			});
	}

	void reloadData(const std::vector<Masina>& masini) {
		model->setMasini(masini);
	}

	/*
	void reloadData(const std::vector<Masina>& masini) {
		tabel->clear();
		tabel->setEditTriggers(QAbstractItemView::NoEditTriggers);

		auto coloane = new QStringList;
		coloane->append("Numar");
		coloane->append("Producator");
		coloane->append("Model");
		coloane->append("Tip");
		tabel->setHorizontalHeaderLabels(*coloane);
		tabel->horizontalHeader()->setStyleSheet("color: white");
		tabel->verticalHeader()->setStyleSheet("color: white");
		tabel->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

		tabel->setStyleSheet("QHeaderView::section {background-color: blue}");
	
		int linie = 0;
		for (const auto& masina : masini) {
			QFont f;
			f.setBold(true);

			auto nrInmatriculare = new QTableWidgetItem{ QString::fromStdString(masina.getNrInmatriculare()) };
			nrInmatriculare->setFont(f);
			nrInmatriculare->setBackground(QBrush{ Qt::magenta, Qt::SolidPattern });
			tabel->setItem(linie, 0, nrInmatriculare);

			auto producator = new QTableWidgetItem{ QString::fromStdString(masina.getProducator()) };
			producator->setFont(f);
			producator->setBackground(QBrush{ Qt::yellow, Qt::SolidPattern });
			tabel->setItem(linie, 1, producator);

			auto model = new QTableWidgetItem{ QString::fromStdString(masina.getModel()) };
			model->setFont(f);
			model->setBackground(QBrush{ Qt::green, Qt::SolidPattern });
			tabel->setItem(linie, 2, model);

			auto tip = new QTableWidgetItem{ QString::fromStdString(masina.getTip()) };
			tip->setFont(f);
			tip->setBackground(QBrush{ Qt::cyan, Qt::SolidPattern });
			tabel->setItem(linie, 3, tip);

			linie++;
		}
	}
	*/

public:
	showWindow(serviceMasina& serv) :service{ serv } {
		initShowWindow();
		connectSignalsSlots();
		//reloadData(service.getAll());
	}
 };