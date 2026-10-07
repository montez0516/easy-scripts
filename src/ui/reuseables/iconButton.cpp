#include "iconButton.hpp"
#include "image.hpp"

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
    text_ = new QLabel(text, this);

    text_->setObjectName("IconButtonText");

    icon_->setAttribute(Qt::WA_TransparentForMouseEvents);
    text_->setAttribute(Qt::WA_TransparentForMouseEvents);

    text_->setAlignment(Qt::AlignCenter);

    layout_->addWidget(icon_, 1);
    layout_->addWidget(text_);
}

void IconButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);

    qDebug() << "IconButton:" << size();
    qDebug() << "Icon:" << icon_->size();
    qDebug() << "Text:" << text_->size();
}

QSize IconButton::sizeHint() const
{
    return QSize(75, 75);
}

QSize IconButton::minimumSizeHint() const
{
    return QSize(50, 50);
}
