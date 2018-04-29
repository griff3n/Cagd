/********************************************************************************
** Form generated from reading UI file 'QTCadg.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTCADG_H
#define UI_QTCADG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "openglwidget.h"

QT_BEGIN_NAMESPACE

class Ui_QTCadgClass
{
public:
    QAction *actionBeenden;
    QWidget *centralWidget;
    OpenGLWidget *openGLWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QMenu *menuDatei;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *QTCadgClass)
    {
        if (QTCadgClass->objectName().isEmpty())
            QTCadgClass->setObjectName(QStringLiteral("QTCadgClass"));
        QTCadgClass->setWindowModality(Qt::ApplicationModal);
        QTCadgClass->resize(752, 536);
        actionBeenden = new QAction(QTCadgClass);
        actionBeenden->setObjectName(QStringLiteral("actionBeenden"));
        centralWidget = new QWidget(QTCadgClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new OpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 50, 751, 451));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 20, 75, 23));
        QTCadgClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QTCadgClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 752, 21));
        menuBar->setDefaultUp(false);
        menuDatei = new QMenu(menuBar);
        menuDatei->setObjectName(QStringLiteral("menuDatei"));
        QTCadgClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTCadgClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTCadgClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuDatei->menuAction());
        menuDatei->addAction(actionBeenden);
        mainToolBar->addSeparator();

        retranslateUi(QTCadgClass);
        QObject::connect(actionBeenden, SIGNAL(triggered()), QTCadgClass, SLOT(closeApplication()));

        QMetaObject::connectSlotsByName(QTCadgClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTCadgClass)
    {
        QTCadgClass->setWindowTitle(QApplication::translate("QTCadgClass", "QTCadg", nullptr));
        actionBeenden->setText(QApplication::translate("QTCadgClass", "Beenden", nullptr));
        pushButton->setText(QApplication::translate("QTCadgClass", "PushButton", nullptr));
        menuDatei->setTitle(QApplication::translate("QTCadgClass", "Datei", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTCadgClass: public Ui_QTCadgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTCADG_H
