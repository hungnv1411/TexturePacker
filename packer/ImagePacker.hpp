#ifndef IMAGEPACKER_HPP
#define IMAGEPACKER_HPP

#include "Packer.hpp"

namespace packer {

class ImagePacker
{
private:
    int prevSortOrder;
    void internalPack(int heur, int w, int h);

    void SortImages(int w, int h);

public:
    QList<InputImage> images;
    QList<QSize> bins;
    ImagePacker();
    bool compareImages(QImage *img1, QImage *img2, int *i, int *j);
    void pack(int heur, int w, int h);

    unsigned AddImgesToBins(int heur, int w, int h);

    void CropLastImage(int heur, int w, int h, bool wh);
    void DivideLastImage(int heur, int w, int h, bool wh);

    void UpdateCrop();

    float GetFillRate();

    void ClearBin(int binIndex);

    int FillBin(int heur, int w, int h, int binIndex);

    QRect crop(const QImage& img);
    void sort();
    void addItem(const QImage& img, const SpritePtr& data, QString path);
    void addItem(QString path, const SpritePtr& data);
    const InputImage* find(const SpritePtr& data);
    void removeId(const SpritePtr& data);
    void realculateDuplicates();
    void clear();
    int compare;
    quint64 area, neededArea;
    int missingImages;
    int mergedImages;
    bool ltr, merge, square, autosize, mergeBF;
    int cropThreshold;
    Border border;
    int extrude;
    int rotate;
    int sortOrder;
    int minFillRate;
    int minTextureSizeX;
    int minTextureSizeY;
};

}

#endif // IMAGEPACKER_HPP
