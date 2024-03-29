#include "serviceMasina.h"

#include<iostream>
#include <assert.h>
#include <algorithm>
#include <fstream>
#include <string>

vector<Masina> serviceMasina::sorteazaService(bool (*compara)(const Masina&, const Masina&)) {
	vector<Masina> list{ repo.getAll() };
	for (int i = 0; i < list.size() - 1; i++)
		for (int j = i + 1; j < list.size(); j++) {
			if (compara(list[i], list[j])) {
				Masina m = list[i];
				list[i] = list[j];
				list[j] = m;
			}
		}
	return list;
}

const vector<Masina> serviceMasina::sorteazaNrInmatriculareService() {
	return sorteazaService(cmpNrInmatriculare);
}

const vector<Masina> serviceMasina::sorteazaTipService() {
	return sorteazaService(cmpTip);
}

const vector<Masina> serviceMasina::sorteazaProducatorAndModelService() {
	return sorteazaService([](const Masina& a, const Masina& b) {
		if (a.getProducator() == b.getProducator()) {
			return cmpModel(a, b);
		}
		return cmpProducator(a, b);
		});
}

vector<Masina> serviceMasina::filtreazaCriteriuService(function<bool(const Masina&)> functie) {
	vector<Masina> list;
	for (const auto& masina : repo.getAll()) {
		if (functie(masina)) {
			list.push_back(masina);
		}
	}
	return list;
}

const vector<Masina> serviceMasina::filtreazaProducatorService(const string& criteriu) {
	return filtreazaCriteriuService([criteriu](const Masina& m) {
		return m.getProducator() == criteriu;
		});
}

const vector<Masina> serviceMasina::filtreazaTipService(const string& tip) {
	return filtreazaCriteriuService([tip](const Masina& m) {
		return m.getTip() == tip;
		});
}

Masina serviceMasina::CautaMasinaService(const string& nrInmatriculare) {
	Masina m{ nrInmatriculare, "Orice", "Orice", "Orice" };
	validator.validate(m);
	return repo.CautaMasinaRepo(nrInmatriculare);
}

void serviceMasina::ModificaMasinaService(const string& nrInmatriculare, const string& producator, const string& model, const string& tip) {
	Masina m{ nrInmatriculare, producator, model, tip };
	validator.validate(m);

	Masina v = repo.CautaMasinaRepo(nrInmatriculare);
	repo.ModificaMasinaRepo(m);

	actiuniUndo.push_back(std::make_unique<UndoModifica>(v, m, repo.all));
}

void serviceMasina::StergeMasinaService(const string& nrInmatriculare) {
	Masina m{ nrInmatriculare, "Orice", "Orice", "Orice" };
	validator.validate(m);
	Masina v = repo.CautaMasinaRepo(nrInmatriculare);
	repo.StergeMasinaRepo(m);

	actiuniUndo.push_back(std::make_unique<UndoSterge>(v, repo.all));
}

void serviceMasina::AdaugaMasinaService(const string& nrInmatriculare, const string& producator, const string& model, const string& tip) {
	Masina m{ nrInmatriculare, producator, model, tip };
	validator.validate(m);
	repo.AdaugaMasinaRepo(m);

	actiuniUndo.push_back(std::make_unique<UndoAdauga>(m, repo.all));
}

void serviceMasina::AdaugaMasinaSpalatorieService(const string& nrInmatriculare) {
	Masina m{ nrInmatriculare, "Orice", "Orice", "Orice" };
	validator.validate(m);
	spalatorie.adaugaMasinaSpalatorieRepo(repo.CautaMasinaRepo(nrInmatriculare));
	notify();
}

void serviceMasina::GolesteMasiniSpalatorieService() {
	spalatorie.golesteMasiniSpalatorieRepo();
	notify();
}

void serviceMasina::GenereazaAleatorMasiniSpalatorieService(const string& numarMasini) {
	if (spalatorie.getAllSpalatorie().size() == repo.getAll().size()) {
		throw("Nu se mai pot genera noi masini diferite!");
	}

	int numar = -1;
	try {
		numar = std::stoi(numarMasini);
	}
	catch (const std::invalid_argument) {
		throw("Introduceti un numar intreg!");
	}

	if (numar <= 0) {
		throw("Numarul de masini trebuie sa fie strict pozitiv!");
	}

	if (numar > repo.getAll().size()) {
		throw("Nu exista suficiente masini pentru a se genera!");
	}
	else if (numar == repo.getAll().size()) {
		for (const auto& masina : repo.getAll()) {
			spalatorie.adaugaMasinaSpalatorieRepo(masina);
		}
		return;
	}

	int len = repo.getAll().size();
	while (numar) {
		int r = rand() % len;
		Masina m = repo.getAll()[r];
		if (!spalatorie.exista(m)) {
			spalatorie.adaugaMasinaSpalatorieRepo(m);
			numar--;
		}
	}

	notify();
}

void serviceMasina::ExportaMasiniSpalatorieService(const string& fisier) {
	spalatorie.ExportaMasiniSpalatorieRepo(fisier);
}

void serviceMasina::undoService() {
	if (actiuniUndo.size() == 0) {
		throw("Nu se mai poate face undo!");
	}

	actiuniUndo.back()->doUndo();
	actiuniUndo.pop_back();
}

/*
void testExportaMasiniSpalatorieService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };


	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");
	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	service.AdaugaMasinaService("B833CCC", "Skoda", "Octavia", "berlina");

	service.AdaugaMasinaSpalatorieService("NT11NIS");
	service.AdaugaMasinaSpalatorieService("NT11RXA");

	service.ExportaMasiniSpalatorieService("testExporta.csv");

	std::string line;
	std::ifstream fin("testExporta.csv");

	std::getline(fin, line);
	assert(line == "Lista masini:");

	std::getline(fin, line);
	assert(line == "NT11NIS, Audi, A4, break");

	std::getline(fin, line);
	assert(line == "NT11RXA, Volkswagen, CC, berlina");

	service.AdaugaMasinaSpalatorieService("CJ55ARC");
	service.AdaugaMasinaSpalatorieService("B833CCC");

	service.ExportaMasiniSpalatorieService("testExporta.csv");

	int count = 0;
	while (std::getline(fin, line)) {
		count++;
	}

	assert(count == 2);
}

void testSorteazaProducatorAndModelService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");
	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	service.AdaugaMasinaService("B833CCC", "Skoda", "Octavia", "berlina");

	assert(service.sorteazaProducatorAndModelService().size() == 4);
	assert(service.sorteazaProducatorAndModelService()[0].getNrInmatriculare() == "NT11NIS");
	assert(service.sorteazaProducatorAndModelService()[1].getProducator() == "Skoda");
	assert(service.sorteazaProducatorAndModelService()[2].getModel() == "Arteon");
	assert(service.sorteazaProducatorAndModelService()[3].getTip() == "berlina");
}

void testSorteazaTipService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");
	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	service.AdaugaMasinaService("B833CCC", "Skoda", "Octavia", "berlina");

	assert(service.sorteazaTipService().size() == 4);
	assert(service.sorteazaTipService()[0].getNrInmatriculare() == "NT11RXA");
	assert(service.sorteazaTipService()[1].getProducator() == "Skoda");
	assert(service.sorteazaTipService()[2].getModel() == "A4");
	assert(service.sorteazaTipService()[3].getTip() == "sport");
}

void testSorteazaNrInmatriculareService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");
	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	service.AdaugaMasinaService("B833CCC", "Skoda", "Octavia", "berlina");

	assert(service.sorteazaNrInmatriculareService().size() == 4);
	assert(service.sorteazaNrInmatriculareService()[0].getNrInmatriculare() == "B833CCC");
	assert(service.sorteazaNrInmatriculareService()[1].getProducator() == "Volkswagen");
	assert(service.sorteazaNrInmatriculareService()[2].getModel() == "A4");
	assert(service.sorteazaNrInmatriculareService()[3].getTip() == "berlina");
}

void testFiltreazaTipService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");
	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	service.AdaugaMasinaService("B833CCC", "Skoda", "Octavia", "berlina");

	assert(service.filtreazaTipService("break").size() == 1);
	assert(service.filtreazaTipService("sport").size() == 1);
	assert(service.filtreazaTipService("berlina").size() == 2);
	assert(service.filtreazaTipService("berlina")[1].getProducator() == "Skoda");
}

void testFiltreazaProducatorService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("NT11NIS", "Audi", "A4", "break");

	assert(service.filtreazaProducatorService("Volkswagen").size() == 1);
	assert(service.filtreazaProducatorService("Volkswagen")[0].getNrInmatriculare() == "NT11RXA");

	assert(service.filtreazaProducatorService("Audi").size() == 1);
	assert(service.filtreazaProducatorService("Audi")[0].getTip() == "break");

	service.AdaugaMasinaService("CJ55ARC", "Volkswagen", "Arteon", "sport");
	assert(service.filtreazaProducatorService("Volkswagen").size() == 2);
}

void testCautaMasinaService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	try {
		Masina m = service.CautaMasinaService("NZ33IOS");
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	assert(service.getAll().size() == 1);
	assert(service.CautaMasinaService("NT11RXA").getNrInmatriculare() == "NT11RXA");
	assert(service.CautaMasinaService("NT11RXA").getProducator() == "Volkswagen");
	assert(service.CautaMasinaService("NT11RXA").getModel() == "CC");
	assert(service.CautaMasinaService("NT11RXA").getTip() == "berlina");

	try {
		Masina m = service.CautaMasinaService("NT11NIS");
	}
	catch (const repoMasinaException&) {
		assert(true);
	}
}

void testModificaMasinaService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");
	assert(service.getAll().size() == 2);
	service.ModificaMasinaService("NT11NIS", "Audi", "A7", "sportback");
	assert(service.CautaMasinaService("NT11NIS").getProducator() == "Audi");
	assert(service.CautaMasinaService("NT11NIS").getModel() == "A7");
	assert(service.CautaMasinaService("NT11NIS").getTip() == "sportback");

	service.ModificaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	assert(service.CautaMasinaService("NT11RXA").getProducator() == "Volkswagen");
	assert(service.CautaMasinaService("NT11RXA").getModel() == "CC");
	assert(service.CautaMasinaService("NT11RXA").getTip() == "berlina");

	try {
		service.ModificaMasinaService("CJ99ABC", "Skoda", "Octavia", "berlina");
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	try {
		service.ModificaMasinaService("CJ", "Skoda", "Octavia", "berlina");
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}
}

void testStergeMasinaService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	try {
		service.StergeMasinaService("SM95SVP");
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	assert(service.getAll().size() == 0);

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");

	assert(service.getAll().size() == 2);
	service.StergeMasinaService("NT11RXA");
	assert(service.getAll().size() == 1);
	try {
		service.StergeMasinaService("NT11N3S");
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}
	assert(service.getAll().size() == 1);
}

void testAdaugaMasinaService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	assert(service.getAll().size() == 1);

	try {
		service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");
	assert(service.getAll().size() == 2);

	try {
		service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "");
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}
}

void testAdaugaMasinaSpalatorieService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");

	assert(service.getAllSpalatorie().size() == 0);
	service.AdaugaMasinaSpalatorieService("NT11RXA");
	assert(service.getAllSpalatorie().size() == 1);

	try {
		service.AdaugaMasinaSpalatorieService("CJ55ABC");
	}
	catch (const repoMasinaException&) {
		assert(true);
	}

	try {
		service.AdaugaMasinaSpalatorieService("abc");
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	service.AdaugaMasinaSpalatorieService("NT11NIS");
	assert(service.getAllSpalatorie().size() == 2);
}

void testGolesteMasiniSpalatorieService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	assert(service.getAllSpalatorie().size() == 0);
	try {
		service.GolesteMasiniSpalatorieService();
	}
	catch (const char*) {
		assert(true);
	}

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");
	service.AdaugaMasinaSpalatorieService("NT11RXA");
	assert(service.getAllSpalatorie().size() == 1);
	service.GolesteMasiniSpalatorieService();
	assert(service.getAllSpalatorie().size() == 0);

	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");

	service.AdaugaMasinaSpalatorieService("NT11RXA");
	service.AdaugaMasinaSpalatorieService("NT11NIS");

	assert(service.getAllSpalatorie().size() == 2);
	service.GolesteMasiniSpalatorieService();
	assert(service.getAllSpalatorie().size() == 0);
}

void testGenereazaAleatorMasiniSpalatorieService() {
	repoMasina repo;
	validatorMasina validator;
	repoSpalatorie spalatorie;
	serviceMasina service{ repo, validator, spalatorie };

	try {
		service.GenereazaAleatorMasiniSpalatorieService("0");
	}
	catch (const char*) {
		assert(true);
	}

	service.AdaugaMasinaService("NT11RXA", "Volkswagen", "CC", "berlina");

	try {
		service.GenereazaAleatorMasiniSpalatorieService("abc");
	}
	catch (const char*) {
		assert(true);
	}

	try {
		service.GenereazaAleatorMasiniSpalatorieService("-1");
	}
	catch (const char*) {
		assert(true);
	}

	try {
		service.GenereazaAleatorMasiniSpalatorieService("4");
	}
	catch (const char*) {
		assert(true);
	}

	service.AdaugaMasinaService("NT11NIS", "Volkswagen", "Golf 4", "break");

	service.GenereazaAleatorMasiniSpalatorieService("1");
	assert(service.getAllSpalatorie().size() == 1);

	service.GenereazaAleatorMasiniSpalatorieService("1");
	assert(service.getAllSpalatorie().size() == 2);

	service.GolesteMasiniSpalatorieService();
	service.GenereazaAleatorMasiniSpalatorieService("2");
	assert(service.getAllSpalatorie().size() == 2);
}

void testService() {
	testAdaugaMasinaService();
	testStergeMasinaService();
	testModificaMasinaService();
	testCautaMasinaService();
	testFiltreazaProducatorService();
	testFiltreazaTipService();
	testSorteazaNrInmatriculareService();
	testSorteazaTipService();
	testSorteazaProducatorAndModelService();
	testAdaugaMasinaSpalatorieService();
	testGolesteMasiniSpalatorieService();
	testGenereazaAleatorMasiniSpalatorieService();
	testExportaMasiniSpalatorieService();
}
*/