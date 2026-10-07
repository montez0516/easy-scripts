#include "navBar.hpp"

#include <QWidget>
#include <QBoxLayout>
#include <QSize>

NavBar::NavBar(QBoxLayout::Direction direction, QWidget *parent) : QWidget(parent), layout_(new QBoxLayout(direction, this))
{
    layout_->setDirection(direction);
    layout_->setContentsMargins(3, 3, 3, 3);
    layout_->setSpacing(8);
}

void NavBar::addWidget(QWidget *widget)
{
    layout_->addWidget(widget, 1);
}