#pragma once

#include <vector>

using std::vector;

class TabelModel : public QAbstractTableModel {
private:
	vector<Masina> masini;

public:
	TabelModel(vector<Masina> m) : masini{ m } {}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override {
		return masini.size();
	}

	int columnCount(const QModelIndex& parent = QModelIndex()) const override {
		return 4;
	}

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override {
		Masina m = masini[index.row()];

		if (role == Qt::DisplayRole) {
			if (index.column() == 0) {
				return QString::fromStdString(m.getNrInmatriculare());
			}
			else if (index.column() == 1) {
				return QString::fromStdString(m.getProducator());
			}
			else if (index.column() == 2) {
				return QString::fromStdString(m.getModel());
			}
			else {
				return QString::fromStdString(m.getTip());
			}
		}

		if (role == Qt::BackgroundRole) {
			if (index.column() == 0) {
				return QBrush{ Qt::magenta, Qt::SolidPattern };
			}
			else if (index.column() == 1) {
				return QBrush{ Qt::yellow, Qt::SolidPattern };
			}
			else if (index.column() == 2) {
				return QBrush{ Qt::green, Qt::SolidPattern };
			}
			else {
				return QBrush{ Qt::cyan, Qt::SolidPattern };
			}
		}

		return QVariant{};

		}

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
			if (role == Qt::DisplayRole) {
				if (orientation == Qt::Horizontal) {
					if (section == 0) {
						return QString("Numar inmatriculare");
					}
					else if (section == 1) {
						return QString("Producator");
					}
					else if (section == 2) {
						return QString("Model");
					}
					else if (section == 3) {
						return QString("Tip");
					}
				}
				else {
					return QString("%1").arg(section + 1);
				}
			}

			return QVariant{};
		}	

		void setMasini(const vector<Masina>& m) {
			this->masini = m;
			auto topLeft = createIndex(0, 0);
			auto bottomRight = createIndex(rowCount(), columnCount());	
			emit dataChanged(topLeft, bottomRight); 
		}
};

