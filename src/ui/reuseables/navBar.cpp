#include "navBar.hpp"

#include <QWidget>
#include <QBoxLayout>
#include <QSize>

NavBar::NavBar(QBoxLayout::Direction direction, QWidget *parent) : QWidget(parent), layout_(new QBoxLayout(direction, this))
{
    layout_->setDirection(direction);
}

void NavBar::addWidget(QWidget *widget)
{
    layout_->addWidget(widget);
}