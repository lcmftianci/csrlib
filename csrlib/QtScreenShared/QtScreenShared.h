#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtScreenShared.h"

class QtScreenShared : public QMainWindow
{
	Q_OBJECT

public:
	QtScreenShared(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtScreenSharedClass ui;
};
