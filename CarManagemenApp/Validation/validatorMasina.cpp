#include "validatorMasina.h"
#include <assert.h>
#include <sstream>

using std::stringstream;

void validatorMasina::validate(const Masina& m) {
	string error = "";

	string nrInmatriculare = m.getNrInmatriculare();

	if (nrInmatriculare.size() != 7 && nrInmatriculare.size() != 6)
		error += "Numar de inmatriculare invalid!\n";
	else {
		for (int i = 0; i < nrInmatriculare.size(); i++) {
			if (nrInmatriculare[i] >= 'a' && nrInmatriculare[i] <= 'z') {
				nrInmatriculare[i] -= 32;
			}
		}

		int nrCaractereInceput = 0, i = 0;
		while (i < nrInmatriculare.size() && nrInmatriculare[i] >= 'A' && nrInmatriculare[i] <= 'Z') {
			nrCaractereInceput++;
			i++;
		}

		int nrCifre = 0;
		while (i < nrInmatriculare.size() && nrInmatriculare[i] >= '0' && nrInmatriculare[i] <= '9') {
			nrCifre++;
			i++;
		}

		int nrCaractereFinal = 0;
		while (i < nrInmatriculare.size() && nrInmatriculare[i] >= 'A' && nrInmatriculare[i] <= 'Z') {
			nrCaractereFinal++;
			i++;
		}

		bool valid = true;

		if (nrCaractereInceput == 2) {
			if (nrCifre != 2) {
				valid = false;
			}
			else {
				if (nrCaractereFinal != 3) {
					valid = false;
				}
			}
		}
		else if (nrCaractereInceput == 1) {
			if (nrCifre != 2 && nrCifre != 3) {
				valid = false;
			}
			else {
				if (nrCaractereFinal != 3) {
					valid = false;
				}
			}
		}
		else {
			valid = false;
		}

		char nr[] = " AB AG AR B BC BH BN BR BT BV BZ CJ CL CS CT CV DB DJ GJ GL GR HD HR IF IL IS MH MM MS NT OT PH SB SJ SM SV TL TM TR VL VN VS ";
		char jud[5];
		if (nrCaractereInceput == 2) {
			jud[0] = ' ';
			jud[1] = nrInmatriculare[0];
			jud[2] = nrInmatriculare[1];
			jud[3] = ' ';
			jud[4] = '\0';
		}
		else {
			jud[0] = ' ';
			jud[1] = nrInmatriculare[0];
			jud[2] = ' ';
			jud[3] = '\0';
		}

		if (strstr(nr, jud) == NULL) {
			valid = false;
		}


		if (!valid) {
			error += "Numarul de inmatriculare este invalid!\n";
		}
	}

	if (m.getProducator() == "" || m.getProducator().size() < 2) {
		error += "Producatorul masinii trebuie sa aiba cel putin 2 caractere!\n";
	}

	if (m.getModel() == "" || m.getModel().size() < 1) {
		error += "Modelul masinii trebuie sa aiba cel putin un caracter!\n";
	}

	if (m.getTip() == "" || m.getTip().size() < 1) {
		error += "Tipul masinii trebuie sa aiba cel putin un caracter!\n";
	}

	if (error.size() > 0)
		throw validatorMasinaException(error);
}

ostream& operator<<(ostream& out, const validatorMasinaException& m) {
	out << m.errors;
	return out;
}

void testValidate() {
	validatorMasina validator;

	Masina a{ "nt11rxa", "VW", "CC", "berlina" };
	validator.validate(a);

	Masina b{ "ZZ23ABC", "VW", "CC", "berlina" };
	try {
		validator.validate(b);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina c{ "a", "VW", "CC", "berlina" };
	try {
		validator.validate(c);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina d{ "NT0ABCD", "VW", "CC", "berlina" };
	try {
		validator.validate(d);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina e{ "AB123CD", "VW", "CC", "berlina" };
	try {
		validator.validate(e);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina f{ "NT11RXA", "a", "CC", "berlina" };
	try {
		validator.validate(f);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina g{ "NT11RXA", "VW", "", "berlina" };
	try {
		validator.validate(g);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina h{ "NT11RXA", "VW", "CC", "" };
	try {
		validator.validate(h);
	}
	catch (const validatorMasinaException& e) {
		assert(true);
		stringstream os;
		os << e;
		assert(os.str().find("Tipul masinii trebuie sa aiba cel putin un caracter!\n") >= 0);
	}

	Masina i{ "B1111CCC", "VW", "CC", "berlina" };
	try {
		validator.validate(i);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina j{ "NUMAR__", "VW", "CC", "berlina" };
	try {
		validator.validate(j);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina k{ "CJ01@B", "VW", "CC", "berlina" };
	try {
		validator.validate(k);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina l{ "B1@AB@2", "VW", "CC", "berlina" };
	try {
		validator.validate(l);
	}
	catch (const validatorMasinaException&) {
		assert(true);
	}

	Masina m{ "B101A@C", "VW", "CC", "berlina" };
	try {
		validator.validate(m);
	}
	catch (const validatorMasinaException& e) {
		stringstream os;
		os << e;
		assert(os.str().find("Numarul de inmatriculare contine prea multe caractere!\n") >= 0);
	}
}