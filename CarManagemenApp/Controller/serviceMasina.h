#pragma once

#include "../Repo/repoMasina.h"
#include "../Repo/repoSpalatorie.h"
#include "../Validation/validatorMasina.h"
#include "../Domain/Masina.h"
#include <string>
#include <vector>
#include <functional>
#include <utility>
#include "../Controller/undo.h"
#include "../Observer/observer.h"

using std::string;
using std::vector;
using std::function;

class serviceMasina : public Observable
{
private:
	repoMasina& repo;
	validatorMasina& validator;
	repoSpalatorie& spalatorie;

	std::vector<std::unique_ptr<ActiuneUndo>> actiuniUndo;

	vector<Masina> filtreazaCriteriuService(function<bool(const Masina&)> functie);

	vector<Masina> sorteazaService(bool (*compara)(const Masina&, const Masina&));

public:
	serviceMasina(repoMasina& repo, validatorMasina& validator, repoSpalatorie& spalatorie) : repo{ repo }, validator{ validator }, spalatorie{ spalatorie } {
	}

	serviceMasina(const serviceMasina& ot) = delete;

	void AdaugaMasinaService(const string& nrInmatriculare, const string& producator, const string& model, const string& tip);

	void StergeMasinaService(const string& nrInmatriculare);

	void ModificaMasinaService(const string& nrInmatriculare, const string& producator, const string& model, const string& tip);

	Masina CautaMasinaService(const string& nrInmatriculare);

	const vector<Masina>& getAll() noexcept {
		return repo.getAll();
	}

	const vector<Masina> filtreazaProducatorService(const string& criteriu);

	const vector<Masina> filtreazaTipService(const string& tip);

	const vector<Masina> sorteazaNrInmatriculareService();

	const vector<Masina> sorteazaTipService();

	const vector<Masina> sorteazaProducatorAndModelService();

	void AdaugaMasinaSpalatorieService(const string& nrInmatriculare);

	const vector<Masina>& getAllSpalatorie() noexcept {
		return spalatorie.getAllSpalatorie();
	}

	void GolesteMasiniSpalatorieService();

	void GenereazaAleatorMasiniSpalatorieService(const string& numarMasini);

	void ExportaMasiniSpalatorieService(const string& fisier);

	void undoService();
};

void testService();