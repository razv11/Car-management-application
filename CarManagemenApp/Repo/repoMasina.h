#pragma once

#include "../Domain/Masina.h"
#include <string>
#include <vector>
#include <ostream>

using std::vector;
using std::string;
using std::ostream;

class repoMasina
{
private:
	string fileName;

	//functie privata - verifica daca exista deja o masina in lista
	bool exista(const Masina& m) const;

	//functie privata - returneaza pozitia unei masini din vectorul dinamic
	int pozitieElement(const Masina& m) const;

	void loadFromFile();

public:
	vector<Masina> all;
	//setam construtorul default
	repoMasina(string fN) :fileName{ fN } {
		loadFromFile();
	}

	//Copy constructor
	repoMasina(const repoMasina& ot) = delete;

	/*
	adauga o maina in vector daca aceasta nu exista
	arunca exceptie daca exista deja masina
	*/
	void AdaugaMasinaRepo(const Masina& m);

	/*
	sterge o masina din vector dupa numarul de inmatriculare
	arunca exceptie daca nu exista masina cu numarul dat
	*/
	void StergeMasinaRepo(const Masina& m);

	/*
	modifica producatorul, modelul si tipul unei masini care are un numar de inmatriculare dat
	arunca exceptie daca nu exita nicio masina cu numarul introdus
	*/
	void ModificaMasinaRepo(const Masina& m);

	/*
	returneaza un obiect de tip Masina care are un numar de inmatriculare dat
	arunca exceptie daca nu exista un astfel de obiect
	*/
	Masina CautaMasinaRepo(const string& nrInmatriculare) const;

	/*
	returnaza lista cu toate obictele de tip Masina existente
	*/
	const vector<Masina>& getAll() const;

	//functii "prietene" deoarece testam doua proceduri private ale clasei 
	friend void testPozitieElement();
	friend void testExista();
};


class repoMasinaException
{
public:
	string errors;

	repoMasinaException(string error) : errors{ error } {
	}

	friend ostream& operator<<(ostream& out, const repoMasinaException& m);
};

//suprascriem operatorul '<<' pentru a afisa exceptiile proprii
ostream& operator<<(ostream& out, const repoMasinaException& m);

void testRepo();