#include "GridWidget.h"

#include "FlowLayout.h"

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QEvent>

#include <algorithm>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    layout_ = new FlowLayout(this, 0, 16, 16);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GridWidget::addWidget(QWidget *widget)
{
    layout_->addWidget(widget);
}
