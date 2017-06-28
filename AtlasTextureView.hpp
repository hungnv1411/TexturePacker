#ifndef ATLASTEXTUREVIEW_HPP
#define ATLASTEXTUREVIEW_HPP

#include <QWidget>

class AtlasTextureView : public QWidget
{
    Q_OBJECT
public:
    AtlasTextureView(QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *event);
private:
    QList<QPixmap> textures;
    float scale;
    QSize size;
    QPixmap pattern;
public slots:
    void updatePixmap(const QList<QImage> &image);
    void rescale(QString);
};

#endif // ATLASTEXTUREVIEW_HPP
