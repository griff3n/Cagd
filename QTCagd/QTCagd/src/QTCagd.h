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

class QTCagd : public QMainWindow
{
	Q_OBJECT

public:
	QTCagd(QWidget *parent = Q_NULLPTR);

public slots:
	void closeApplication();
	void openFile();
	void parsingDone();
	void showSelectedVertexProperties(graphicVertex*);
	void xCoordChanged(double);
	void yCoordChanged(double);
	void zCoordChanged(double);

signals:
	void meshChanged(HalfEdgeMesh*);
	void vertexMoved();

private:
	Ui::QTCagdClass ui;
	QFutureWatcher<HalfEdgeMesh*> parseWatcher;
	graphicVertex* selectedVertex = nullptr;
};

#endif // QTCAGD_H