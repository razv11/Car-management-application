#pragma once
#include "../Domain/Masina.h"
#include <vector>
#include <algorithm>
#include <iostream>

class ActiuneUndo {
public:
    virtual void doUndo() = 0;  // Metoda pur virtuala
    virtual ~ActiuneUndo() {}   // Distructor virtual pentru a evita memory leaks
};


class UndoAdauga : public ActiuneUndo {
private:
    Masina masina;
    std::vector<Masina>& lista;

public:
    UndoAdauga(Masina m, std::vector<Masina>& lista) : masina{ m }, lista{ lista } {}

    void doUndo() override {
        lista.erase(std::find(lista.begin(), lista.end(), masina));
    }
};


class UndoSterge : public ActiuneUndo {
private:
    Masina masina;
    std::vector<Masina>& lista;

public:
    UndoSterge(Masina m, std::vector<Masina>& lista) : masina{ m }, lista{ lista } {}

    void doUndo() override {
        lista.push_back(masina);
    }
};


class UndoModifica : public ActiuneUndo {
private:
    Masina vechi;
    Masina nou;
    std::vector<Masina>& lista;

public:
    UndoModifica(Masina v, Masina n, std::vector<Masina>& lista) : vechi{ v }, nou{ n }, lista{ lista } {}

    void doUndo() override {
        std::replace(lista.begin(), lista.end(), nou, vechi);
    }
};

