#include "ImagePacker.hpp"
#include <QDebug>
#include "MaxRects.hpp"

#include <QHash>

#define CMPF1(x,y,a) qAlpha(img.pixel(x,y)) > a
#define CMP(x,y,a) if(CMPF1(x,y,a)) {t = false; break;} if(!t) break;
#define CMPF2(x,y) img.pixel(x,y) != pix
#define CMP2(x,y) if(CMPF2(x,y)) {t = false; break;} if(!t) break;

namespace packer {

ImagePacker::ImagePacker() {

}

bool ImagePacker::pack(ConfigurationsPtr& configs)
{
    this->configs = configs;
    if (configs == nullptr) return false;

    sortImages();

    missingImages = 1;
    mergedImages = 0;
    area = 0;
    bins.clear();

    unsigned areaBuf = addImgesToBins();

    if (areaBuf && !missingImages) {
        cropLastImage(false);
    }

    if (configs->merge) {
        for(int i = 0; i < sprites.size(); i++) {
            if(sprites.at(i)->duplicatedSprite != nullptr)
            {
                sprites[i]->pos = find(sprites.at(i)->duplicatedSprite)->pos;
                sprites[i]->textureId = find(sprites.at(i)->duplicatedSprite)->textureId;
                mergedImages++;
            }
        }
    }
}

void ImagePacker::updateCrop() {
    for(int i = 0; i < sprites.size(); ++i) {
        sprites[i]->crop = crop(QImage(sprites[i]->path));
    }
}

void ImagePacker::clear()
{
    sprites.clear();
}

void ImagePacker::realculateDuplicates()
{
    for(int i = 0; i < sprites.count(); i++)
    {
        sprites[i]->duplicatedSprite = NULL;
    }
    for(int i = 0; i < sprites.count(); i++)
    {
        for(int k = i + 1; k < sprites.count(); k++)
        {
            if(sprites.at(k)->duplicatedSprite == NULL &&
                    sprites.at(i)->hash == sprites.at(k)->hash &&
                    sprites.at(i)->size == sprites.at(k)->size &&
                    sprites.at(i)->crop == sprites.at(k)->crop)
            {
                sprites[k]->duplicatedSprite = sprites.at(i);
            }
        }
    }
}

void ImagePacker::removeId(const SpritePtr& data)
{
    for(int k = 0; k < sprites.count(); k++)
    {
        if(sprites.at(k) == data)
        {
            sprites.removeAt(k);
            break;
        }
    }
}
const SpritePtr ImagePacker::find(const SpritePtr& data)
{
    for(int i = 0; i < sprites.count(); i++)
    {
        if(data == sprites.at(i))
        {
            return sprites.at(i);
        }
    }
    return nullptr;
}

void ImagePacker::sortImages()
{
    realculateDuplicates();
    neededArea = 0;
    int w = configs->atlasWidth;
    int h = configs->atlasHeight;
    QSize size;
    for(int i = 0; i < sprites.size(); i++)
    {
        sprites.at(i)->pos = QPoint(999999, 999999);
        if(configs->cropThreshold)
        {
            size = sprites.at(i)->crop.size();
        }
        else
        {
            size = sprites.at(i)->size;
        }
        if(size.width() == w)
        {
            size.setWidth(size.width() - configs->border.left - configs->border.right - 2 * configs->extrude);
        }
        if(size.height() == h)
        {
            size.setHeight(size.height() - configs->border.top - configs->border.bottom - 2 * configs->extrude);
        }
        size += QSize(configs->border.left + configs->border.right + 2 * configs->extrude,
                      configs->border.top + configs->border.bottom + 2 * configs->extrude);

        sprites[i]->rotated = false;
        if((configs->rotate == ROTATION_WIDTH_GREATHER_HEIGHT && size.width() > size.height()) ||
                (configs->rotate == ROTATION_WIDTH_GREATHER_2HEIGHT && size.width() > 2 * size.height()) ||
                (configs->rotate == ROTATION_HEIGHT_GREATHER_WIDTH && size.height() > size.width()) ||
                (configs->rotate == ROTATION_H2_WIDTH_H && size.height() > size.width() &&
                 size.width() * 2 > size.height()) ||
                (configs->rotate == ROTATION_W2_HEIGHT_W && size.width() > size.height() &&
                 size.height() * 2 > size.width()) ||
                (configs->rotate == ROTATION_HEIGHT_GREATHER_2WIDTH && size.height() > 2 * size.width()))
        {
            size.transpose();
            sprites.at(i)->rotated = true;
        }
        sprites.at(i)->sizeCurrent = size;
        if(sprites.at(i)->duplicatedSprite == nullptr || !configs->merge)
        {
            neededArea += size.width() * size.height();
        }
    }
    sort();
}

int ImagePacker::fillBin(int binIndex)
{
    int w = configs->atlasWidth;
    int h = configs->atlasHeight;
    int heur = configs->heuristic;

    int areaBuf = 0;
    MaxRects rects;
    MaxRectsNode mrn;
    mrn.r = QRect(0, 0, w, h);
    rects.F << mrn;
    rects.heuristic = heur;
    rects.leftToRight = ltr;
    rects.w = w;
    rects.h = h;
    rects.rotation = configs->rotate;
    rects.border = &configs->border;
    for(int i = 0; i < sprites.size(); i++)
    {
        if(QPoint(999999, 999999) != sprites.at(i)->pos)
        {
            continue;
        }
        if(sprites.at(i)->duplicatedSprite == nullptr || !configs->merge)
        {
            sprites[i]->pos = rects.insertNode(sprites[i]);
            sprites[i]->textureId = binIndex;
            if(QPoint(999999, 999999) == sprites.at(i)->pos)
            {
                missingImages++;
            }
            else
            {
                areaBuf += sprites.at(i)->sizeCurrent.width() * sprites.at(i)->sizeCurrent.height();
                area += sprites.at(i)->sizeCurrent.width() * sprites.at(i)->sizeCurrent.height();
            }
        }
    }
    return areaBuf;
}

void ImagePacker::clearBin(int binIndex)
{
    for(int i = 0; i < sprites.size(); i++)
    {
        if(sprites.at(i)->textureId == binIndex)
        {
            area -= sprites.at(i)->sizeCurrent.width() * sprites.at(i)->sizeCurrent.height();
            sprites.at(i)->pos = QPoint(999999, 999999);
        }
    }
}

unsigned ImagePacker::addImgesToBins()
{
    int w = configs->atlasWidth;
    int h = configs->atlasHeight;

    int binIndex = bins.count() - 1;
    unsigned areaBuf = 0;
    unsigned lastAreaBuf = 0;
    do
    {
        missingImages = 0;
        bins << QSize(w, h);
        lastAreaBuf = fillBin(++binIndex);
        if(!lastAreaBuf)
        {
            bins.removeLast();
        }
        areaBuf += lastAreaBuf;
    }
    while(missingImages && lastAreaBuf);
    return areaBuf;
}

void ImagePacker::cropLastImage(bool wh) {
    int w = configs->atlasWidth;
    int h = configs->atlasHeight;
    int heur = configs->heuristic;

    missingImages = 0;
    QList<SpritePtr> last_images = sprites;
    QList<QSize> last_bins = bins;
    quint64 last_area = area;

    bins.removeLast();
    clearBin(bins.count());

    if(configs->square) {
        w /= 2;
        h /= 2;
    } else {
        if(wh) {
            w /= 2;
        } else {
            h /= 2;
        }
        wh = !wh;
    }

    int binIndex = bins.count();
    missingImages = 0;
    bins << QSize(w, h);
    fillBin(binIndex);
    if(missingImages) {
        sprites = last_images;
        bins = last_bins;
        area = last_area;
        missingImages = 0;
        if(configs->square) {
            w *= 2;
            h *= 2;
        } else {
            if(!wh) {
                w *= 2;
            } else {
                h *= 2;
            }
            wh = !wh;
        }
        if(configs->autosize)
        {
            float rate = getFillRate();
            if((rate < (static_cast<float>(configs->minFillRate) / 100.f)) && ((w > configs->minTextureSizeX) && (h > configs->minTextureSizeY)))
            {
                divideLastImage(wh);
                if(getFillRate() <= rate)
                {
                    sprites = last_images;
                    bins = last_bins;
                    area = last_area;
                }
            }
        }
    } else {
        cropLastImage(wh);
    }
}

void ImagePacker::divideLastImage(bool wh)
{
    int w = configs->atlasWidth;
    int h = configs->atlasHeight;
    int heur = configs->heuristic;

    missingImages = 0;
    QList<SpritePtr> last_images = sprites;
    QList<QSize> last_bins = bins;
    quint64 last_area = area;

    bins.removeLast();
    clearBin(bins.count());

    if(configs->square) {
        w /= 2;
        h /= 2;
    } else {
        if(wh) {
            w /= 2;
        } else {
            h /= 2;
        }
        wh = !wh;
    }
    addImgesToBins();
    if(missingImages){
        sprites = last_images;
        bins = last_bins;
        area = last_area;
        missingImages = 0;
    } else {
        cropLastImage(wh);
    }
}

float ImagePacker::getFillRate()
{
    quint64 binArea = 0;
    for(int i = 0; i < bins.count(); i++)
    {
        binArea += bins.at(i).width() * bins.at(i).height();
    }
    return (float)((double)area / (double)binArea);
}

//auto-cropping algorithm
QRect ImagePacker::crop(const QImage &img)
{
    int j, w, h, x, y;
    QRgb pix;
    bool t;
    //crop all
    if(false)
    {
        QHash<QRgb, int> hash;
        hash[img.pixel(0, 0)]++;
        hash[img.pixel(img.width() - 1, 0)]++;
        hash[img.pixel(img.width() - 1, img.height() - 1)]++;
        hash[img.pixel(0, img.height() - 1)]++;
        QHashIterator<QRgb, int> i(hash);
        while(i.hasNext())
        {
            i.next();
            if(i.value() > 2)
            {
                pix = i.key();
                t = true;
                for(y = 0; y < img.height(); y++)
                {
                    for(j = 0; j < img.width(); j++)
                        CMP2(j, y)
                    }
                t = true;
                for(x = 0; x < img.width(); x++)
                {
                    for(j = y; j < img.height(); j++)
                        CMP2(x, j)
                    }
                t = true;
                for(w = img.width(); w > 0; w--)
                {
                    for(j = y; j < img.height(); j++)
                        CMP2(w - 1, j)
                    }
                t = true;
                for(h = img.height(); h > 0; h--)
                {
                    for(j = x; j < w; j++)
                        CMP2(j, h - 1)
                    }
                goto found_by_color;
            }
        }
    }
    //crop only alpha
    t = true;
    for(y = 0; y < img.height(); y++)
    {
        for(j = 0; j < img.width(); j++)
            CMP(j, y, configs->cropThreshold)
        }
    t = true;
    for(x = 0; x < img.width(); x++)
    {
        for(j = y; j < img.height(); j++)
            CMP(x, j, configs->cropThreshold)
        }
    t = true;
    for(w = img.width(); w > 0; w--)
    {
        for(j = y; j < img.height(); j++)
            CMP(w - 1, j, configs->cropThreshold)
        }
    t = true;
    for(h = img.height(); h > 0; h--)
    {
        for(j = x; j < w; j++)
            CMP(j, h - 1, configs->cropThreshold)
        }
found_by_color:
    w = w - x;
    h = h - y;
    if(w < 0)
    {
        w = 0;
    }
    if(h < 0)
    {
        h = 0;
    }
    return QRect(x, y, w, h);
}

bool ImageCompareByHeight(const SpritePtr &i1, const SpritePtr &i2)
{
    return (i1->sizeCurrent.height() << 10) + i1->sizeCurrent.width() >
           (i2->sizeCurrent.height() << 10) + i2->sizeCurrent.width();
}
bool ImageCompareByWidth(const SpritePtr &i1, const SpritePtr &i2)
{
    return (i1->sizeCurrent.width() << 10) + i1->sizeCurrent.height() >
           (i2->sizeCurrent.width() << 10) + i2->sizeCurrent.height();
}
bool ImageCompareByArea(const SpritePtr &i1, const SpritePtr &i2)
{
    return i1->sizeCurrent.height() * i1->sizeCurrent.width() >
           i2->sizeCurrent.height() * i2->sizeCurrent.width();
}

bool ImageCompareByMax(const SpritePtr &i1, const SpritePtr &i2)
{
    int first = i1->sizeCurrent.height() > i1->sizeCurrent.width() ?
                i1->sizeCurrent.height() : i1->sizeCurrent.width();
    int second = i2->sizeCurrent.height() > i2->sizeCurrent.width() ?
                 i2->sizeCurrent.height() : i2->sizeCurrent.width();
    if(first == second)
    {
        return ImageCompareByArea(i1, i2);
    }
    else
    {
        return first > second;
    }
}

void ImagePacker::sort()
{
    switch(configs->sortOrder)
    {
    case 1:
        qSort(sprites.begin(), sprites.end(), ImageCompareByWidth);
        break;
    case 2:
        qSort(sprites.begin(), sprites.end(), ImageCompareByHeight);
        break;
    case 3:
        qSort(sprites.begin(), sprites.end(), ImageCompareByArea);
        break;
    case 4:
        qSort(sprites.begin(), sprites.end(), ImageCompareByMax);
        break;
    }
}

}


