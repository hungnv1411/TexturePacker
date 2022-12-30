#ifndef PACKER_HPP
#define PACKER_HPP

#include <QtCore>
#include <QImage>
#include "../utils/Utils.hpp"

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
    quint32 hash;
    QString path;
    QImage texture;
    int textureId;
    Sprite* duplicatedSprite;

    QPoint pos;
    QSize size;
    QSize sizeCurrent;
    QRect crop;
    bool cropped;
    bool rotated;

    Sprite(const QString& path) {
        this->path = path;
        this->texture = QImage(path);
        this->hash = Utils::hash(0, texture.bits(), texture.sizeInBytes() );
        textureId = -1;
        duplicatedSprite = NULL;
        size = texture.size();
    }

    bool operator ==(const Sprite& other) {
        return this->hash == other.hash;
    }
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
        sortOrder = SORT_MAX;
        heuristic = HEURISTIC_TL;
        extrude = 1;
        merge = true;
        square = false;
        autosize = true;
        minFillRate = 80;
        mergeBF = false;
        rotate = ROTATION_ONLY_WHEN_NEEDED;
        cropThreshold = 10;
        minTextureSizeX = 32;
        minTextureSizeY = 32;

        atlasHeight = 2048;
        atlasWidth = 2048;
    }

    int atlasWidth;
    int atlasHeight;

    int sortOrder;
    Border border;
    int heuristic;
    int extrude;
    bool merge;
    bool square;
    bool autosize;
    int minFillRate;
    bool mergeBF;
    int rotate;
    int cropThreshold;
    int minTextureSizeX;
    int minTextureSizeY;
};

typedef QSharedPointer<Configurations> ConfigurationsPtr;

}

#endif // PACKER_HPP
