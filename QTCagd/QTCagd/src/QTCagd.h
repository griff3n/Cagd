#pragma once

#ifndef QTCAGD_H
#define QTCAGD_H

#include <GL/glew.h>
#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>
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
	void openFile();
	void parsingDone();

signals:
	void meshChanged(HalfEdgeMesh*);

private:
	Ui::QTCagdClass ui;
	QFutureWatcher<HalfEdgeMesh*> parseWatcher;
};

#endif // QTCAGD_H