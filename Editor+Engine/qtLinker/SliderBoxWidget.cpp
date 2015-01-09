#include "TreeWidget.h"
#include <iostream>
#include <QCoreApplication>
#include <qtconcurrentrun.h>
#include <QThread>
#include <qtextobject.h>
#include "SliderBoxWidget.h"


SliderBoxWidget::SliderBoxWidget(QString name, int val, QWidget *parent) :
QWidget(parent)
{
    resize(1280, 960);

    p = parent;
    w = width();
    h = height();

    f = 0;
    vLayout = new QHBoxLayout(this);
    Title = new QLineEdit(name);
    Title->setDisabled(true);
    vLayout->addWidget(Title, 0, Qt::AlignLeft);
    Title2 = new QSlider();
    Title2->setValue(val);
    Title2->setMinimum(-200);
    Title2->setMaximum(200);
    vLayout->addWidget(Title2, 0, Qt::AlignLeft);
}
SliderBoxWidget::~SliderBoxWidget()
{
//	_sleep(10);
};
void SliderBoxWidget::paintEvent(QPaintEvent * event)
{

    //if ((f % 1 == 0) && (w != p->width() || h != p->height()))
    //{
    //	float h2 = p->height();
    //	float w2 = p->width();

    //	h2 /= h;
    //	w2 /= w;
    //	if (h2 == 1 && w2 == 1)
    //	{
    //	}
    //	else{
    //		if (h2 != 0 && w2 != 0)
    //		{
    //			//scroll->setFixedSize(scroll->width() * w2, scroll->height() * h2);
    //			//mside->setFixedHeight(mside->height() * h2);
    //			this->setFixedSize(p->width()/* * w2*/ - 90, p->height() - 25 /** h2*/);
    //		}
    //	}
    //	w = p->width();
    //	h = p->height();
    //	f = 0;
    //};
    //f++;
}
