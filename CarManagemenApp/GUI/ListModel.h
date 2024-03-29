#pragma once

#include <QtWidgets/qabstractitemview.h>

class MyListModel : public QAbstractListModel {
private:
	vector<Masina> masini;

public:
	MyListModel(vector<Masina> m) : masini{ m } {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return masini.size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 1;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		Masina m = masini[index.row()];

		if (role == Qt::DisplayRole) {
			return QString::fromStdString(m.getNrInmatriculare());
		}

		return QVariant{};
	}

	void setMasini(vector<Masina> m) {
		masini = m;
		auto topLeft = createIndex(0, 0);
		auto bottomRight = createIndex(rowCount(), columnCount());
		emit dataChanged(topLeft, bottomRight);
	}

};