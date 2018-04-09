#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtVideoShow.h"

class QtVideoShow : public QMainWindow
{
	Q_OBJECT

public:
	QtVideoShow(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtVideoShowClass ui;
};
