#include "repoSpalatorie.h"

void repoSpalatorie::ExportaMasiniSpalatorieRepo(const string& fisier) {
	string numeFisier = "./Repo/" + fisier;
 	std::ofstream fout(numeFisier);

	fout << "Lista masini:\n";
	for (const auto& masina : lista) {
		fout << masina.getNrInmatriculare() << ", " << masina.getProducator() << ", " << masina.getModel() << ", " << masina.getTip() << '\n';
	}

	fout.close();
}

bool repoSpalatorie::exista(const Masina& m) const {
	for (const auto& masina : lista) {
		if (masina.getNrInmatriculare() == m.getNrInmatriculare())
			return true;
	}
	return false;
}

void repoSpalatorie::adaugaMasinaSpalatorieRepo(const Masina& m) {
	if (!exista(m)) {
		lista.push_back(m);
		return;
	}
	throw("Aceasta masina exista deja in lista cu masini pentru spalat!");
}

void repoSpalatorie::golesteMasiniSpalatorieRepo() {
	if (!lista.size()) {
		throw("Lista este deja goala!");
	}
	lista.clear();
}

const vector<Masina>& repoSpalatorie::getAllSpalatorie() const {
	return lista;
}

void testadaugaMasinaSpalatorieRepo() {
	Masina m{ "NT11RXA", "Volkswagen", "CC", "berlina" };

	repoSpalatorie repo;
	repo.adaugaMasinaSpalatorieRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });

	try {
		repo.adaugaMasinaSpalatorieRepo(Masina{ "NT11RXA", "Volkswagen", "CC", "berlina" });
	}
	catch (const char*) {
		assert(true);
	}
}

void testRepoSpalatorie() {
	testadaugaMasinaSpalatorieRepo();
}