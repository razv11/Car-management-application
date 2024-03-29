#pragma once

#include "../Controller/serviceMasina.h"

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qformlayout.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlabel.h>

class searchWindow : public QWidget {
private:
	serviceMasina& service;

	QLineEdit* searchLine = new QLineEdit;
	QLineEdit* resultLine = new QLineEdit;

	QPushButton* btnSearch = new QPushButton{ "&Cauta" };
	QPushButton* btnCancel = new QPushButton{ "&Cancel" };

	void initSearchWindow() {
		setMinimumWidth(550);
		setWindowTitle("Search car");

		QVBoxLayout* mainLayout = new QVBoxLayout;
		setLayout(mainLayout);

		QHBoxLayout* formsLayout = new QHBoxLayout;
		{
			QVBoxLayout* formSearchLayout = new QVBoxLayout;
			QLabel* searchLabel = new QLabel{ "Numar inmatriculare" };
			formSearchLayout->addWidget(searchLabel);
			formSearchLayout->addWidget(searchLine);

			QVBoxLayout* formResultLayout = new QVBoxLayout;
			QLabel* resultLabel = new QLabel{ "Masina cautata" };
			formResultLayout->addWidget(resultLabel);
			resultLine->setDisabled(true);
			formResultLayout->addWidget(resultLine);

			formsLayout->addLayout(formSearchLayout);
			formsLayout->addSpacing(50);
			formsLayout->addLayout(formResultLayout);
		}
		mainLayout->addLayout(formsLayout);
		mainLayout->addSpacing(30);

		QHBoxLayout* buttonsLayout = new QHBoxLayout;
		buttonsLayout->addStretch();
		buttonsLayout->addWidget(btnSearch);
		buttonsLayout->addWidget(btnCancel);	
		mainLayout->addLayout(buttonsLayout);
	}

	void connectSignalsSlots() {
		QObject::connect(btnSearch, &QPushButton::clicked, [&]() {
			auto nrInmatriculare = searchLine->text();
			try {
				Masina m = service.CautaMasinaService(nrInmatriculare.toStdString());
				string details = m.getNrInmatriculare() + " - " + m.getProducator() + " - " + m.getModel() + " - " + m.getTip();
				resultLine->setText(QString::fromStdString(details));
				searchLine->setText("");
			}
			catch (const validatorMasinaException& validationError) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(validationError.errors));
			}
			catch (const repoMasinaException& repoError) {
				QMessageBox::warning(nullptr, "Warning", QString::fromStdString(repoError.errors));
			}

			});

		QObject::connect(btnCancel, &QPushButton::clicked, [&]() {
			close();
			
			});
	}

public:
	searchWindow(serviceMasina& serv) : service{ serv } {
		initSearchWindow();
		connectSignalsSlots();
	}
};