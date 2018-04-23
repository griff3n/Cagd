#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTCadg.h"

class QTCadg : public QMainWindow
{
	Q_OBJECT

public:
	QTCadg(QWidget *parent = Q_NULLPTR);

private:
	Ui::QTCadgClass ui;
};
