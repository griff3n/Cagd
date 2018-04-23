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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTCadgClass
{
public:
    QWidget *centralWidget;
    QOpenGLWidget *openGLWidget;
    QPushButton *pushButton;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *QTCadgClass)
    {
        if (QTCadgClass->objectName().isEmpty())
            QTCadgClass->setObjectName(QStringLiteral("QTCadgClass"));
        QTCadgClass->resize(603, 404);
        centralWidget = new QWidget(QTCadgClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        openGLWidget = new QOpenGLWidget(centralWidget);
        openGLWidget->setObjectName(QStringLiteral("openGLWidget"));
        openGLWidget->setGeometry(QRect(0, 60, 601, 291));
        pushButton = new QPushButton(centralWidget);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(50, 20, 75, 23));
        QTCadgClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QTCadgClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 603, 21));
        QTCadgClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTCadgClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTCadgClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        mainToolBar->addSeparator();

        retranslateUi(QTCadgClass);

        QMetaObject::connectSlotsByName(QTCadgClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTCadgClass)
    {
        QTCadgClass->setWindowTitle(QApplication::translate("QTCadgClass", "QTCadg", nullptr));
        pushButton->setText(QApplication::translate("QTCadgClass", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QTCadgClass: public Ui_QTCadgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTCADG_H
