#pragma once

#include <string>

using std::string;

class Masina
{
private:
	string nrInmatriculare;
	string producator;
	string model;
	string tip;

public:
	//constructorul pentru Masina: nrInmatriculare, producator, model, tip
	Masina(const string nr, const string p, const string m, const string t) : nrInmatriculare{ nr }, producator{ p }, model{ m }, tip{ t } {
	}

	//returneaza numarul de inmatriculare masinii
	string getNrInmatriculare() const {
		return nrInmatriculare;
	}

	//returneaza producatorul masinii
	string getProducator() const {
		return producator;
	}

	//returneaza modelul masinii
	string getModel() const {
		return model;
	}

	//returneaza tip masinii
	string getTip() const {
		return tip;
	}

	int operator==(Masina m) {
		return m.getNrInmatriculare() == nrInmatriculare;
	}

	void operator=(Masina const& m) {
		nrInmatriculare = m.getNrInmatriculare();
		producator = m.getProducator();
		model = m.getModel();
		tip = m.getTip();
	}
};

bool cmpNrInmatriculare(const Masina& a, const Masina& b);

bool cmpTip(const Masina& a, const Masina& b);

bool cmpProducator(const Masina& a, const Masina& b);

bool cmpModel(const Masina& a, const Masina& b);
