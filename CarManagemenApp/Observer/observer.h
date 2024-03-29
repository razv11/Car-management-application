#pragma once

#include <vector>

using std::vector;

class Observer {
public:
	virtual void update() = 0;
};


class Observable {
private:
	vector<Observer*> obs;

public:
	void addObserver(Observer* o) {
		obs.push_back(o);
	}

	void removeObserver(Observer* o) {
		obs.erase(std::remove(obs.begin(), obs.end(), o), obs.end());
	}

protected:
	void notify() {
		for (auto o : obs) {
			o->update();
		}
	}

};