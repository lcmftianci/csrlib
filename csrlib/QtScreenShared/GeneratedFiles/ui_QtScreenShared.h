/********************************************************************************
** Form generated from reading UI file 'QtScreenShared.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSCREENSHARED_H
#define UI_QTSCREENSHARED_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtScreenSharedClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtScreenSharedClass)
    {
        if (QtScreenSharedClass->objectName().isEmpty())
            QtScreenSharedClass->setObjectName(QStringLiteral("QtScreenSharedClass"));
        QtScreenSharedClass->resize(600, 400);
        menuBar = new QMenuBar(QtScreenSharedClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QtScreenSharedClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtScreenSharedClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtScreenSharedClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QtScreenSharedClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QtScreenSharedClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtScreenSharedClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtScreenSharedClass->setStatusBar(statusBar);

        retranslateUi(QtScreenSharedClass);

        QMetaObject::connectSlotsByName(QtScreenSharedClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtScreenSharedClass)
    {
        QtScreenSharedClass->setWindowTitle(QApplication::translate("QtScreenSharedClass", "QtScreenShared", 0));
    } // retranslateUi

};

namespace Ui {
    class QtScreenSharedClass: public Ui_QtScreenSharedClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSCREENSHARED_H
