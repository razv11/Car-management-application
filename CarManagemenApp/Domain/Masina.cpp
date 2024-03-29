#include "masina.h"

bool cmpNrInmatriculare(const Masina& a, const Masina& b) {
	return a.getNrInmatriculare() > b.getNrInmatriculare();
}

bool cmpTip(const Masina& a, const Masina& b) {
	return a.getTip() > b.getTip();
}

bool cmpProducator(const Masina& a, const Masina& b) {
	return a.getProducator() > b.getProducator();
}

bool cmpModel(const Masina& a, const Masina& b) {
	return a.getModel() > b.getModel();
}