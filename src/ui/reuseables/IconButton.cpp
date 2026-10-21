#include "IconButton.h"
#include "Image.h"

#include <QPushButton>
#include <QString>
#include <QLabel>
#include <QVBoxLayout>
#include <Qt>

IconButton::IconButton(
    const QString &text,
    const QString &icon,
    QWidget *parent) : QPushButton(parent)
{
    setObjectName("IconButton");
    setCursor(Qt::PointingHandCursor);

    setSizePolicy(
        QSizePolicy::Expanding,
        QSizePolicy::Expanding);

    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(8, 6, 8, 6);
    layout_->setSpacing(2);

    icon_ = new Image(icon, this);
    icon_->setAttribute(Qt::WA_TransparentForMouseEvents);
    layout_->addWidget(icon_, 1);

    if (!text.isEmpty())
    {
        text_ = new QLabel(text, this);

        text_->setObjectName("IconButtonText");

        text_->setAttribute(Qt::WA_TransparentForMouseEvents);
        text_->setAlignment(Qt::AlignCenter);
        layout_->addWidget(text_);
    }
}

void IconButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);
}

QSize IconButton::sizeHint() const
{
    return QSize(75, 75);
}

QSize IconButton::minimumSizeHint() const
{
    return QSize(50, 50);
}

void IconButton::setIcon(const QString &icon)
{
    icon_->changeSource(icon);
}