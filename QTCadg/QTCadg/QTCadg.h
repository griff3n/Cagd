#ifndef QTCAGD_H
#define QTCAGD_H

#include <QtWidgets/QMainWindow>
#include "ui_QTCadg.h"

class QTCadg : public QMainWindow
{
	Q_OBJECT

public:
	QTCadg(QWidget *parent = Q_NULLPTR);

public slots:
	void closeApplication();

private:
	Ui::QTCadgClass ui;
};

#endif // QTCAGD_H