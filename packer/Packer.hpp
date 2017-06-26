#ifndef PACKER_HPP
#define PACKER_HPP

#include <QtCore>
#include <QImage>

namespace packer {

struct Border {
    int top, left, bottom, right;
    Border() {
        top = left = bottom = right = 0;
    }
};

struct trbl {
    QPoint t, r, b, l;
};

struct MaxRectsNode {
    QRect r; //rect
    trbl b; //border
};

struct PackedImage
{
    QImage img;
    QRect rc;
    QRect crop;
    bool border, rotate;
    int textureId;
    int id;
};

struct InputImage
{
    quint32 hash;
    int textureId;
    void *id;
    void *duplicateId;
    QPoint pos;
    QSize size, sizeCurrent;
    QRect crop;
    QString path;

    bool cropped, rotated;
};

struct Configurations {
    bool isRecursive;
    QStringList folders;
    QStringList files;

    Configurations() {
        isRecursive = true;
    }
};

}

#endif // PACKER_HPP
