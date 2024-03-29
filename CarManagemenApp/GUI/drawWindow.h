#pragma once

#include "../Controller/serviceMasina.h"
#include <math.h>

#include <QtWidgets/qwidget.h>
#include <qpainter.h>

class drawWindow : public QWidget, public Observer {
private:
	serviceMasina& service;

public:
	drawWindow(serviceMasina& serv) : service{ serv } {
		service.addObserver(this);
	}

	void update() override {
		repaint();
	}

	void paintEvent(QPaintEvent* ev) override {
		QPainter p{ this };
		for (const auto& masina : service.getAllSpalatorie()) {
			int x = rand() % height() / 2;
			int y = rand() % width() / 2;

			if (x % 2 == 0) {
				p.drawRect(x, y, 40, 70);
			}
			else if (x % 3 == 0) {
				p.drawLine(x, y, 50, 100);
			}
			else {
				p.drawEllipse(QPoint(x, y), 30, 30);
			}
		}
	}

};