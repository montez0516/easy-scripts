#include "scriptGrid.hpp"
#include "scriptCard.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QSize>
#include <QEvent>

#include <algorithm>

ScriptGrid::ScriptGrid(QWidget *parent) : QWidget(parent)
{
    setObjectName("ScriptGrid");

    layout_ = new QGridLayout(this);
    layout_->setSpacing(16);
    layout_->setContentsMargins(0, 0, 0, 0);
}

void ScriptGrid::addScript(ScriptCard *scriptCard)
{
    scripts_.append(scriptCard);
    updateGrid();
}

void ScriptGrid::updateGrid()
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
    for (int i = 0; i < scripts_.size(); ++i)
    {
        int row = i / columns;
        int column = i % columns;

        layout_->addWidget(scripts_[i], row, column);
    }
}

void ScriptGrid::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updateGrid();
}