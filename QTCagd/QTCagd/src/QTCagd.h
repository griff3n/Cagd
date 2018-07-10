#pragma once

#ifndef QTCAGD_H
#define QTCAGD_H

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

private:
	void saveFile(QString filename);

public slots:
	void closeApplication();
	void openFile();
	void save();
	void saveAs();
	void parsingDone();
	void showSelectedVertexProperties(graphicVertex*);
	void xCoordChanged(double);
	void yCoordChanged(double);
	void zCoordChanged(double);
	void vertexMode(bool toggled);
	void edgeMode(bool toggled);
	void faceMode(bool toggled);
	void catmullTool();

private:
	Ui::QTCagdClass ui;
	QFutureWatcher<HalfEdgeMesh*> parseWatcher;
	graphicVertex* selectedVertex = nullptr;
	HalfEdgeMesh* mesh = nullptr;
	QString openedFile;
};

#endif // QTCAGD_H