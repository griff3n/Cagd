#pragma once

#ifndef QTCAGD_H
#define QTCAGD_H

#include <GL/glew.h>
#include <QtWidgets/QMainWindow>
#include "ui_QTCagd.h"
#include "ObjectLoader.h"
#include "Orb.h"

class QTCagd : public QMainWindow
{
	Q_OBJECT

public:
	QTCagd(QWidget *parent = Q_NULLPTR);

public slots:
	void closeApplication();
	void showBunny();

signals:
	void meshChanged(HalfEdgeMesh*);

private:
	Ui::QTCagdClass ui;
};

#endif // QTCAGD_H