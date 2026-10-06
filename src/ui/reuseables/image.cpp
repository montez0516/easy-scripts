#include "image.hpp"

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QImage>
#include <QVBoxLayout>

#include <string>

Image::Image(const std::string &src, QWidget *parent) : QWidget(parent), source_(src)
{
    layout_ = new QVBoxLayout(this);
}