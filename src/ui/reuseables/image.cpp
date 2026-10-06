#include "image.hpp"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QString>
#include <Qt>

Image::Image(const QString &src, QWidget *parent)
    : QWidget(parent),
      source_(src),
      pixmap_(src)
{
    layout_ = new QVBoxLayout(this);
    layout_->setContentsMargins(0, 0, 0, 0);

    image_ = new QLabel(this);
    image_->setAlignment(Qt::AlignCenter);

    layout_->addWidget(image_);

    updatePixmap();
}

void Image::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    updatePixmap();
}

void Image::updatePixmap()
{
    if (pixmap_.isNull())
        return;

    image_->setPixmap(
        pixmap_.scaled(
            image_->size(),
            Qt::KeepAspectRatio,
            Qt::SmoothTransformation));
}