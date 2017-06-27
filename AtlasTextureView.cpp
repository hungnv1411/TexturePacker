#include "AtlasTextureView.hpp"
#include <QDebug>
#include <QPainter>

AtlasTextureView::AtlasTextureView(QWidget *parent) : QWidget(parent)
{
    scale = 1;
    size = QSize(0, 0);
}

void AtlasTextureView::paintEvent(QPaintEvent * /* event */)
{
    QPainter painter(this);
    int minHeight = 0;
    for(int i = 0; i < textures.count(); i++)
    {
        minHeight += (textures[i].size().height() + 10) * scale;
    }
    this->setMinimumSize(size.width()*scale, minHeight);
    int pos = 0;
    for(int i = 0; i < textures.count(); i++)
    {
        painter.fillRect(0, pos, textures.at(i).width()*scale,
                         textures.at(i).height()*scale, Qt::magenta);
        painter.drawPixmap(0, pos, textures.at(i).width()*scale,
                           textures.at(i).height()*scale, textures.at(i));
        pos += (textures.at(i).height() + 10) * scale;
    }
}


void AtlasTextureView::updatePixmap(const QList<QImage> &images)
{
    textures.clear();
    QPixmap texture;
    for(int i = 0; i < images.count(); i++)
    {
        texture = QPixmap::fromImage(images.at(i));
        if(i == 0)
        {
            size = texture.size();
        }
        textures << texture;
    }
    this->setMinimumSize(size.width()*scale,
                         (size.height() + 10)*images.count()*scale);
    update();
}

void AtlasTextureView::rescale(QString s)
{
    scale = (float)s.remove('%').toInt() / 100.f;
    if(scale == 0.f)
    {
        scale = 1.f;
    }
    this->update();
}
