#include "gridWidget.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QEvent>

#include <algorithm>

GridWidget::GridWidget(QWidget *parent) : QWidget(parent)
{
    layout_ = new QGridLayout(this);
    layout_->setSpacing(16);
    layout_->setContentsMargins(0, 0, 0, 0);
}

void GridWidget::addWidget(QWidget *widget)
{
    widgets_.append(widget);
    updateGrid();
}

void GridWidget::updateGrid()
{

    constexpr int minimumScriptWidth = 250;
    int width = this->width();

    int columns = std::max(2, (width + layout_->horizontalSpacing()) / (minimumScriptWidth + layout_->horizontalSpacing()));

    while (QLayoutItem *item = layout_->takeAt(0))
    {
        // Don't delete the actual widgets
        delete item;
    }

    // Re-add them based on the new column count
    for (int i = 0; i < widgets_.size(); ++i)
    {
        int row = i / columns;
        int column = i % columns;

        layout_->addWidget(widgets_[i], row, column);
    }
}

void GridWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateGrid();
}