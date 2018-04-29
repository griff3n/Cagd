#ifndef QTCAGD_H
#define QTCAGD_H

#include <QtWidgets/QMainWindow>
#include "ui_QTCagd.h"

class QTCagd : public QMainWindow
{
	Q_OBJECT

public:
	QTCagd(QWidget *parent = Q_NULLPTR);

public slots:
	void closeApplication();

private:
	Ui::QTCagdClass ui;
};

#endif // QTCAGD_H