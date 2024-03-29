#include "repoMasina.h"
#include <assert.h>
#include <sstream>
#include <fstream>

using std::stringstream;

void repoMasina::loadFromFile() {
	std::ifstream fin(fileName);

	string line;
	while (std::getline(fin, line)) {
		int i = 0;

		string nr;
		while (line[i] != ';' && line[i] != ' ') {
			nr += line[i];
			i++;
		}
		while (line[i] == ' ' || line[i] == ';') {
			i++;
		}

		string prod;
		while (line[i] != ';' && line[i] != ' ') {
			prod += line[i];
			i++;
		}
		while (line[i] == ' ' || line[i] == ';') {
			i++;
		}

		string model;
		while (line[i] != ';' && line[i] != ' ') {
			model += line[i];
			i++;
		}
		while (line[i] == ' ' || line[i] == ';') {
			i++;
		}

		string tip;
		while (line[i] != ';' && line[i] != ' ') {
			tip += line[i];
			i++;
		}

		Masina m{ nr, prod, model, tip };

		all.push_back(m);
	}

	fin.close();
}

void repoMasina::ModificaMasinaRepo(const Masina& m) {
	if (!exista(m)) {
		throw repoMasinaException("Nu exista aceasta masina cu numarul " + m.getNrInmatriculare());
	}
	all[pozitieElement(m)] = m;
}

void repoMasina::StergeMasinaRepo(const Masina& m) {
	if (!exista(m)) {
		throw repoMasinaException("Nu exista masina cu numarul " + m.getNrInmatriculare() + " inregistrata");
	}
	all.erase(all.begin() + pozitieElement(m));
}

void repoMasina::AdaugaMasinaRepo(const Masina& m) {
	if (exista(m)) {
		throw repoMasinaException("Exista deja aceasta masina!");
	}
	all.push_back(m);
}

Masina repoMasina::CautaMasinaRepo(const string& nrInmatriculare) const {
	for (const auto& masina : all) {
		if (masina.getNrInmatriculare() == nrInmatriculare)
			return masina;
	}
	throw repoMasinaException("Nu exista masina cu numarul de inmatriculare " + nrInmatriculare);
}

bool repoMasina::exista(const Masina& m) const {
	try {
		Masina masina = CautaMasinaRepo(m.getNrInmatriculare());
		return true;
	}
	catch (repoMasinaException&) {
		return false;
	}
}

int repoMasina::pozitieElement(const Masina& m) const {
	for (int i = 0; i < all.size(); i++)
		if (all[i].getNrInmatriculare() == m.getNrInmatriculare()) {
			return i;
		}
	return -1;
}

const vector<Masina>& repoMasina::getAll() const {
	return all;
}

ostream& operator<<(ostream& out, const repoMasinaException& m) {
	out << m.errors;
	return out;
}

/*
void testModificaMasinaRepo() {
	repoMasina rep;
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 1);
	assert(rep.CautaMasinaRepo("NT11RXA").getNrInmatriculare() == "NT11RXA");
	assert(rep.CautaMasinaRepo("NT11RXA").getProducator() != "Audi");

	rep.ModificaMasinaRepo(Masina{ "NT11RXA", "Audi", "TT", "sport" });
	assert(rep.getAll().size() == 1);
	assert(rep.CautaMasinaRepo("NT11RXA").getProducator() == "Audi");
	assert(rep.CautaMasinaRepo("NT11RXA").getModel() == "TT");
	assert(rep.CautaMasinaRepo("NT11RXA").getTip() == "sport");

	try {
		rep.ModificaMasinaRepo(Masina{ "NT11ABC", "Audi", "TT", "sport" });
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	rep.ModificaMasinaRepo(Masina{ "NT11RXA", "Audi", "TT", "sport" });
	assert(rep.getAll().size() == 1);
	assert(rep.CautaMasinaRepo("NT11RXA").getProducator() == "Audi");
}

void testStergeMasinaRepo() {
	repoMasina rep;
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 1);
	rep.StergeMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 0);

	try {
		rep.StergeMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	rep.AdaugaMasinaRepo(Masina{ "SM95SVP", "Tesla", "S", "sport" });
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 2);

	rep.StergeMasinaRepo(Masina{ "SM95SVP", "Tesla", "S", "sport" });
	rep.StergeMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 0);
}

void testAdaugaMasinaRepo() {
	repoMasina rep;
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 1);
	assert(rep.CautaMasinaRepo("NT11RXA").getNrInmatriculare() == "NT11RXA");
	assert(rep.CautaMasinaRepo("NT11RXA").getProducator() == "Volkswagen");
	assert(rep.CautaMasinaRepo("NT11RXA").getModel() == "CC");
	assert(rep.CautaMasinaRepo("NT11RXA").getTip() == "berlina");

	rep.AdaugaMasinaRepo(Masina{ "NT11NIS", "Volkswagen", "Golf 4", "break" });
	assert(rep.getAll().size() == 2);

	try {
		rep.AdaugaMasinaRepo(Masina{ "NT11NIS", "Volkswagen", "Golf 4", "break" });
	}
	catch (const repoMasinaException& e) {
		stringstream os;
		os << e;
		assert(os.str().find("Exista deja aceasta masina!") >= 0);
	}
}

void testCautaMasinaRepo() {
	repoMasina rep;
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });

	assert(rep.CautaMasinaRepo("NT11RXA").getProducator() == "Volkswagen");
	assert(rep.CautaMasinaRepo("NT11RXA").getModel() == "CC");
	assert(rep.CautaMasinaRepo("NT11RXA").getTip() == "berlina");

	try {
		rep.CautaMasinaRepo("SM11RXA");
	}
	catch (const repoMasinaException& e) {
		stringstream os;
		os << e;
		assert(os.str().find("Nu exista masina cu numarul de inmatriculare SM11RXA") >= 0);
	}
}

void testPozitieElement() {
	repoMasina rep;
	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.pozitieElement(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" }) == 0);

	rep.AdaugaMasinaRepo(Masina{ "NT12RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.pozitieElement(Masina{ "NT12RXA", "Volkswagen", "CC", "berlina" }) == 1);

	assert(rep.pozitieElement(Masina{ "NT15RXA", "Volkswagen", "CC", "berlina" }) == -1);
	rep.AdaugaMasinaRepo(Masina{ "NT15RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.pozitieElement(Masina{ "NT15RXA", "Volkswagen", "CC", "berlina" }) == 2);

	rep.StergeMasinaRepo(Masina{ "NT15RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.pozitieElement(Masina{ "NT15RXA", "Volkswagen", "CC", "berlina" }) == -1);
}

void testExista() {
	repoMasina rep;
	assert(rep.exista(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" }) == false);

	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.exista(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" }) == true);

	rep.StergeMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.exista(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" }) == false);
}

void testGetAll() {
	repoMasina rep;
	assert(rep.getAll().size() == 0);

	rep.AdaugaMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 1);
	assert(rep.getAll()[0].getNrInmatriculare() == "NT11RXA");
	assert(rep.getAll()[0].getModel() == "CC");

	rep.StergeMasinaRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	assert(rep.getAll().size() == 0);
}

void testRepo() {
	testGetAll();
	testExista();
	testPozitieElement();
	testCautaMasinaRepo();
	testAdaugaMasinaRepo();
	testStergeMasinaRepo();
	testModificaMasinaRepo();
}
*/