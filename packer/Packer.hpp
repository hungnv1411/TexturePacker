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

struct Sprite {
    QString path;

    bool operator ==(const Sprite& other) {
        return this->path.compare(other.path, Qt::CaseInsensitive) == 0;
    }
};

typedef QSharedPointer<Sprite> SpritePtr;

struct InputImage
{
    quint32 hash;
    int textureId;
    SpritePtr id;
    SpritePtr duplicateId;
    QPoint pos;
    QSize size, sizeCurrent;
    QRect crop;
    QString path;

    bool cropped, rotated;
};

enum {
    METHOD_GUILLOTINE,
    METHOD_MAXRECTS
}; //method

enum {
    HEURISTIC_NONE,
    HEURISTIC_TL,
    HEURISTIC_BAF,
    HEURISTIC_BSSF,
    HEURISTIC_BLSF,
    HEURISTIC_MINW,
    HEURISTIC_MINH,
    HEURISTIC_NUM
}; //heuristic

enum {
    SORT_NONE,
    SORT_WIDTH,
    SORT_HEIGHT,
    SORT_AREA,
    SORT_MAX,
    SORT_NUM
}; //sort

enum {
    ROTATION_NEVER,
    ROTATION_ONLY_WHEN_NEEDED,
    ROTATION_H2_WIDTH_H,
    ROTATION_WIDTH_GREATHER_HEIGHT,
    ROTATION_WIDTH_GREATHER_2HEIGHT,
    ROTATION_W2_HEIGHT_W,
    ROTATION_HEIGHT_GREATHER_WIDTH,
    ROTATION_HEIGHT_GREATHER_2WIDTH,
    ROTATION_NUM
}; //rotation

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
