#include "ImagePacker.hpp"
#include <QDebug>
#include "MaxRects.hpp"

#include <QHash>

namespace packer {

ImagePacker::ImagePacker() {

}

bool ImagePacker::pack(ConfigurationsPtr& configs)
{
    this->configs = configs;
    if (configs == nullptr) return false;

    updateCrop();
    sortImages(configs->atlasWidth, configs->atlasHeight);

    missingImages = 1;
    mergedImages = 0;
    area = 0;
    bins.clear();

    unsigned areaBuf = addImgesToBins(configs->atlasWidth, configs->atlasHeight);

    if (areaBuf && !missingImages) {
        cropLastImage(configs->atlasWidth, configs->atlasHeight, false);
    }

    if (configs->merge) {
        for(int i = 0; i < sprites.size(); i++) {
            if(sprites[i].duplicatedSprite != nullptr)
            {
                const Sprite* found = find(sprites[i].duplicatedSprite);
                if (found != NULL) {
                    sprites[i].pos = found->pos;
                    sprites[i].textureId = found->textureId;
                }
                mergedImages++;
            }
        }
    }

    return true;
}

void ImagePacker::updateCrop() {
    for(int i = 0; i < sprites.size(); ++i) {
        sprites[i].crop = Utils::crop(sprites[i].texture, configs->cropThreshold);
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
        sprites.operator [](i).duplicatedSprite = NULL;
    }
    for(int i = 0; i < sprites.count(); i++)
    {
        for(int k = i + 1; k < sprites.count(); k++)
        {
            if(sprites.at(k).duplicatedSprite == NULL &&
                    sprites.at(i).hash == sprites.at(k).hash &&
                    sprites.at(i).size == sprites.at(k).size &&
                    sprites.at(i).crop == sprites.at(k).crop)
            {
                sprites.operator [](k).duplicatedSprite = &sprites[i];
            }
        }
    }
}

void ImagePacker::removeId(Sprite* data)
{
    for(int k = 0; k < sprites.count(); k++)
    {
        if(sprites.at(k).hash == data->hash)
        {
            sprites.removeAt(k);
            break;
        }
    }
}
const Sprite* ImagePacker::find(Sprite* data)
{
    for(int i = 0; i < sprites.count(); i++)
    {
        if(data->hash == sprites.at(i).hash)
        {
            return &sprites.at(i);
        }
    }
    return NULL;
}

void ImagePacker::sortImages(int w, int h)
{
    realculateDuplicates();
    neededArea = 0;
    QSize size;
    for(int i = 0; i < sprites.size(); i++)
    {
        sprites.operator [](i).pos = QPoint(999999, 999999);
        if(configs->cropThreshold)
        {
            size = sprites.at(i).crop.size();
        }
        else
        {
            size = sprites.at(i).size;
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

        sprites.operator [](i).rotated = false;
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
            sprites.operator [](i).rotated = true;
        }
        sprites.operator [](i).sizeCurrent = size;
        if(sprites.at(i).duplicatedSprite == NULL || !configs->merge)
        {
            neededArea += size.width() * size.height();
        }
    }
    sort();
}

int ImagePacker::fillBin(int w, int h, int binIndex)
{
    int areaBuf = 0;
    MaxRects rects;
    MaxRectsNode mrn;
    mrn.r = QRect(0, 0, w, h);
    rects.F << mrn;
    rects.heuristic = configs->heuristic;
    rects.leftToRight = ltr;
    rects.w = w;
    rects.h = h;
    rects.rotation = configs->rotate;
    rects.border = &configs->border;
    for(int i = 0; i < sprites.size(); i++)
    {
        if(QPoint(999999, 999999) != sprites.at(i).pos)
        {
            continue;
        }
        if(sprites.at(i).duplicatedSprite == NULL || !configs->merge)
        {
            sprites.operator [](i).pos = rects.insertNode(sprites.operator [](i));
            sprites.operator [](i).textureId = binIndex;
            if(QPoint(999999, 999999) == sprites.operator [](i).pos)
            {
                missingImages++;
            }
            else
            {
                areaBuf += sprites.at(i).sizeCurrent.width() * sprites.at(i).sizeCurrent.height();
                area += sprites.at(i).sizeCurrent.width() * sprites.at(i).sizeCurrent.height();
            }
        }
    }
    return areaBuf;
}

void ImagePacker::clearBin(int binIndex)
{
    for(int i = 0; i < sprites.size(); i++)
    {
        if(sprites.at(i).textureId == binIndex)
        {
            area -= sprites.at(i).sizeCurrent.width() * sprites.at(i).sizeCurrent.height();
            sprites.operator [](i).pos = QPoint(999999, 999999);
        }
    }
}

unsigned ImagePacker::addImgesToBins(int w, int h)
{
    int binIndex = bins.count() - 1;
    unsigned areaBuf = 0;
    unsigned lastAreaBuf = 0;
    do
    {
        missingImages = 0;
        bins << QSize(w, h);
        lastAreaBuf = fillBin(w , h , ++binIndex);
        if(!lastAreaBuf)
        {
            bins.removeLast();
        }
        areaBuf += lastAreaBuf;
    }
    while(missingImages && lastAreaBuf);
    return areaBuf;
}

void ImagePacker::cropLastImage(int w, int h, bool wh)
{
    missingImages = 0;
    QList<Sprite> last_images = sprites;
    QList<QSize> last_bins = bins;
    quint64 last_area = area;

    bins.removeLast();
    clearBin(bins.count());

    if(configs->square)
    {
        w /= 2;
        h /= 2;
    }
    else
    {
        if(wh)
        {
            w /= 2;
        }
        else
        {
            h /= 2;
        }
        wh = !wh;
    }

    int binIndex = bins.count();
    missingImages = 0;
    bins << QSize(w, h);
    fillBin(w , h , binIndex);
    if(missingImages)
    {
        sprites = last_images;
        bins = last_bins;
        area = last_area;
        missingImages = 0;
        if(configs->square)
        {
            w *= 2;
            h *= 2;
        }
        else
        {
            if(!wh)
            {
                w *= 2;
            }
            else
            {
                h *= 2;
            }
            wh = !wh;
        }
        if(configs->autosize)
        {
            float rate = getFillRate();
            if((rate < (static_cast<float>(configs->minFillRate) / 100.f)) &&
                    ((w > configs->minTextureSizeX) && (h > configs->minTextureSizeY)))
            {
                divideLastImage(w, h, wh);
                if(getFillRate() <= rate)
                {
                    sprites = last_images;
                    bins = last_bins;
                    area = last_area;
                }
            }
        }
    }
    else
    {
        cropLastImage(w, h, wh);
    }
}

void ImagePacker::divideLastImage(int w, int h, bool wh)
{
    missingImages = 0;
    QList<Sprite> last_images = sprites;
    QList<QSize> last_bins = bins;
    quint64 last_area = area;

    bins.removeLast();
    clearBin(bins.count());

    if(configs->square)
    {
        w /= 2;
        h /= 2;
    }
    else
    {
        if(wh)
        {
            w /= 2;
        }
        else
        {
            h /= 2;
        }
        wh = !wh;
    }
    addImgesToBins(w, h);
    if(missingImages)
    {
        sprites = last_images;
        bins = last_bins;
        area = last_area;
        missingImages = 0;
    }
    else
    {
        cropLastImage(w, h, wh);
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

bool ImageCompareByHeight(const Sprite &i1, const Sprite &i2)
{
    return (i1.sizeCurrent.height() << 10) + i1.sizeCurrent.width() >
           (i2.sizeCurrent.height() << 10) + i2.sizeCurrent.width();
}
bool ImageCompareByWidth(const Sprite &i1, const Sprite &i2)
{
    return (i1.sizeCurrent.width() << 10) + i1.sizeCurrent.height() >
           (i2.sizeCurrent.width() << 10) + i2.sizeCurrent.height();
}
bool ImageCompareByArea(const Sprite &i1, const Sprite &i2)
{
    return i1.sizeCurrent.height() * i1.sizeCurrent.width() >
           i2.sizeCurrent.height() * i2.sizeCurrent.width();
}

bool ImageCompareByMax(const Sprite &i1, const Sprite &i2)
{
    int first = i1.sizeCurrent.height() > i1.sizeCurrent.width() ?
                i1.sizeCurrent.height() : i1.sizeCurrent.width();
    int second = i2.sizeCurrent.height() > i2.sizeCurrent.width() ?
                 i2.sizeCurrent.height() : i2.sizeCurrent.width();
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
        std::sort(sprites.begin(), sprites.end(), ImageCompareByWidth);
        break;
    case 2:
        std::sort(sprites.begin(), sprites.end(), ImageCompareByHeight);
        break;
    case 3:
        std::sort(sprites.begin(), sprites.end(), ImageCompareByArea);
        break;
    case 4:
        std::sort(sprites.begin(), sprites.end(), ImageCompareByMax);
        break;
    }
}

}


