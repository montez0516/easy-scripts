#include "flowLayout.hpp"

#include <QLayout>
#include <QWidget>
#include <Qt>
#include <QStyle>
#include <QList>
#include <QLayoutItem>
#include <QSize>
#include <QRect>

#include <algorithm>

FlowLayout::FlowLayout(QWidget *parent, int margin, int hSpacing, int vSpacing)
    : QLayout(parent), m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::FlowLayout(int margin, int hSpacing, int vSpacing)
    : m_hSpace(hSpacing), m_vSpace(vSpacing)
{
    setContentsMargins(margin, margin, margin, margin);
}

FlowLayout::~FlowLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

void FlowLayout::addItem(QLayoutItem *item)
{
    itemList.append(item);
}

int FlowLayout::horizontalSpacing() const
{
    if (m_hSpace >= 0)
    {
        return m_hSpace;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
    }
}

int FlowLayout::verticalSpacing() const
{
    if (m_vSpace >= 0)
    {
        return m_vSpace;
    }
    else
    {
        return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
    }
}

int FlowLayout::count() const
{
    return itemList.size();
}

QLayoutItem *FlowLayout::itemAt(int index) const
{
    return itemList.value(index);
}

QLayoutItem *FlowLayout::takeAt(int index)
{
    if (index >= 0 && index < itemList.size())
        return itemList.takeAt(index);
    return nullptr;
}

Qt::Orientations FlowLayout::expandingDirections() const
{
    return {};
}

bool FlowLayout::hasHeightForWidth() const
{
    return true;
}

int FlowLayout::heightForWidth(int width) const
{
    int height = doLayout(QRect(0, 0, width, 0), true);
    return height;
}

void FlowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);
    doLayout(rect, false);
}

QSize FlowLayout::sizeHint() const
{
    return minimumSize();
}

QSize FlowLayout::minimumSize() const
{
    QSize size;
    for (const QLayoutItem *item : std::as_const(itemList))
        size = size.expandedTo(item->minimumSize());

    const QMargins margins = contentsMargins();
    size += QSize(margins.left() + margins.right(), margins.top() + margins.bottom());
    return size;
}

int FlowLayout::doLayout(const QRect &rect, bool testOnly) const
{
    constexpr int minimumCardWidth = 220;
    constexpr int cardHeight = 150;

    int spacing = horizontalSpacing();
    int availableWidth = rect.width();

    int columns = std::max(
        1,
        (availableWidth + spacing) /
            (minimumCardWidth + spacing));

    // No need for more columns than widgets
    columns = std::min(
        columns,
        std::max(1, static_cast<int>(itemList.size())));

    int cardWidth =
        (availableWidth - (columns - 1) * spacing) / columns;

    for (int i = 0; i < itemList.size(); ++i)
    {
        int row = i / columns;
        int column = i % columns;

        int x = rect.x() + column * (cardWidth + spacing);
        int y = rect.y() + row * (cardHeight + verticalSpacing());

        if (!testOnly)
        {
            itemList[i]->setGeometry(
                QRect(x, y, cardWidth, cardHeight));
        }
    }

    int rows = (itemList.size() + columns - 1) / columns;

    return rows * cardHeight +
           std::max(0, rows - 1) * verticalSpacing();
}

int FlowLayout::smartSpacing(QStyle::PixelMetric pm) const
{
    QObject *parent = this->parent();
    if (!parent)
    {
        return -1;
    }
    else if (parent->isWidgetType())
    {
        QWidget *pw = static_cast<QWidget *>(parent);
        return pw->style()->pixelMetric(pm, nullptr, pw);
    }
    else
    {
        return static_cast<QLayout *>(parent)->spacing();
    }
}