#pragma once

#include "../Domain/Masina.h"
#include <ostream>

using std::ostream;

class validatorMasina
{
public:
	void validate(const Masina& m);
};


class validatorMasinaException
{

public:
	string errors;

	validatorMasinaException(string error) : errors{ error } {
	}

	friend ostream& operator<<(ostream& out, const validatorMasinaException& m);
};

//suprascriem operatorul '<<' pentru a afisa exceptiile proprii
ostream& operator<<(ostream& out, const validatorMasinaException& m);

void testValidate();