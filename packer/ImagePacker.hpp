#ifndef IMAGEPACKER_HPP
#define IMAGEPACKER_HPP

#include "Packer.hpp"

namespace packer {

class ImagePacker
{
public:
    ImagePacker();

    void addSprite(const Sprite& sprite) {
        sprites.append(sprite);
    }

    void addSprites(const QList<Sprite>& sprites) {
        this->sprites.append(sprites);
    }

    bool pack(ConfigurationsPtr& configs);

private:
    void sortImages(int w, int h);

public:
    QList<Sprite> sprites;
    QList<QSize> bins;

    unsigned addImgesToBins(int w, int h);
    void cropLastImage(int w, int h, bool wh);
    void divideLastImage(int w, int h, bool wh);
    void updateCrop();
    float getFillRate();
    void clearBin(int binIndex);
    int fillBin(int w, int h, int binIndex);

    void sort();
    const Sprite* find(Sprite* data);
    void removeId(Sprite* data);
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
