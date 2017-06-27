#ifndef IMAGEPACKER_HPP
#define IMAGEPACKER_HPP

#include "Packer.hpp"

namespace packer {

class ImagePacker
{
public:
    ImagePacker();

    void addSprite(const SpritePtr& sprite) {
        sprites.append(sprite);
    }

    void addSprites(const QList<SpritePtr>& sprites) {
        this->sprites.append(sprites);
    }

    bool pack(ConfigurationsPtr& configs);

private:
    void internalPack(int heur, int w, int h);
    void sortImages();

public:
    QList<SpritePtr> sprites;
    QList<QSize> bins;

    unsigned addImgesToBins();
    void cropLastImage(bool wh);
    void divideLastImage(bool wh);
    void updateCrop();
    float getFillRate();
    void clearBin(int binIndex);
    int fillBin(int binIndex);

    QRect crop(const QImage& img);
    void sort();
    const SpritePtr find(const SpritePtr& data);
    void removeId(const SpritePtr& data);
    void realculateDuplicates();
    void clear();
    int compare;
    quint64 area, neededArea;
    int missingImages;
    int mergedImages;
    bool ltr;

private:
    ConfigurationsPtr configs;
};

}

#endif // IMAGEPACKER_HPP
