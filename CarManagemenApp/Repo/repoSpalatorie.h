#pragma once

#include "../Domain/Masina.h"
#include <algorithm>
#include <iterator>
#include <vector>
#include <assert.h>
#include <fstream>	

using std::vector;

class repoSpalatorie
{
private:
	vector<Masina> lista;

public:
	//verifica daca o masina exista deja in lista
	bool exista(const Masina& m) const;

	//adauga o masina in lista cu masini pentru spalat
	void adaugaMasinaSpalatorieRepo(const Masina& m);

	//goleste lista cu masini adaugate pentru spalat
	void golesteMasiniSpalatorieRepo();

	//returneaza toate masinile din lista
	const vector<Masina>& getAllSpalatorie() const;

	//exporta masinile din lista intr-un fisier care exista sau pe care il creeaza
	void ExportaMasiniSpalatorieRepo(const string& fisier);
};

//functa cu teste pentru clasa repoSpalatorie
void testRepoSpalatorie();
