#include "Utils.hpp"
#include <QtCore>
#include <QImage>

QStringList Utils::supportedImageFormats = {"png", "jpg", "jpeg", "gif"};
QStringList Utils::supportedImageFormatFilters = {"*.png", "*.jpg", "*.jpeg", "*.gif"};

#define CMPF1(x,y,a) qAlpha(img.pixel(x,y)) > a
#define CMP(x,y,a) if(CMPF1(x,y,a)) {t = false; break;} if(!t) break;
#define CMPF2(x,y) img.pixel(x,y) != pix
#define CMP2(x,y) if(CMPF2(x,y)) {t = false; break;} if(!t) break;

bool Utils::isImage(const QString &filePath) {
    QString ext = getFileExtension(filePath).toLower();
    return supportedImageFormats.contains(ext);
}

QString Utils::getFileName(const QString &filePath) {
    int idx = filePath.lastIndexOf(QDir::separator());
    if (idx != -1) {
        return filePath.right(filePath.length() - idx - 1);
    }
    return filePath;
}

QString Utils::getContainingDirectory(const QString& filePath) {
    int idx = filePath.lastIndexOf(QDir::separator());
    if (idx != -1) {
        return filePath.left(idx + 1);
    }
    return filePath;
}

QString Utils::getFileExtension(const QString &filePath) {
    int idx = filePath.lastIndexOf(".");
    if (idx != -1) {
        return filePath.right(filePath.length() - idx - 1);
    }
    return "";
}

QStringList Utils::enumerateContentInDirectory(const QString &directory, bool recursive) {
    QDir dir(directory);
    QStringList ret;

    QFileInfoList files = dir.entryInfoList(supportedImageFormatFilters, QDir::Files);
    for (int i = 0; i < files.size(); i++) {
        ret.append(QDir::toNativeSeparators(files[i].absoluteFilePath()));
    }

    if (recursive) {
        QFileInfoList dirs = dir.entryInfoList(QDir::Filter::NoDotAndDotDot | QDir::Dirs);
        for (int i = 0; i < dirs.size(); i++) {
            QFileInfo info = dirs[i];
            QString path = info.absoluteFilePath();
            ret.append(enumerateContentInDirectory(path, recursive));
        }
    }
    return ret;
}

quint32 Utils::hash(quint32 crc, const uchar *buf, size_t len) {
    static quint32 table[256];
    static int have_table = 0;
    quint32 rem, octet;
    const uchar *p, *q;

    /* This check is not thread safe; there is no mutex. */
    if(have_table == 0)
    {
        /* Calculate CRC table. */
        for(int i = 0; i < 256; i++)
        {
            rem = i;  /* remainder from polynomial division */
            for(int j = 0; j < 8; j++)
            {
                if(rem & 1)
                {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                {
                    rem >>= 1;
                }
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = buf + len;
    for(p = buf; p < q; p++)
    {
        octet = *p;  /* Cast to unsigned octet. */
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}

QRect Utils::crop(const QImage &img, int cropThreshold) {
    int j, w, h, x, y;
    QRgb pix;
    //QImage im;
    bool t;
    //crop all
    if(false)
    {
        //    qDebug("%d", img->depth());
        QHash<QRgb, int> hash;
        hash[img.pixel(0, 0)]++;
        hash[img.pixel(img.width() - 1, 0)]++;
        hash[img.pixel(img.width() - 1, img.height() - 1)]++;
        hash[img.pixel(0, img.height() - 1)]++;
        QHashIterator<QRgb, int> i(hash);
        while(i.hasNext())
        {
            i.next();
            //        qDebug("%d %d %d %d - %d", qRed(i.key()), qGreen(i.key()), qBlue(i.key()), qAlpha(i.key()), i.value());
            if(i.value() > 2)
            {
                pix = i.key();
                //            qDebug("%d %d %d %d - %d", qRed(i.key()), qGreen(i.key()), qBlue(i.key()), qAlpha(i.key()), i.value());
                //~ if(qAlpha(pix) == 0) break;
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
            CMP(j, y, cropThreshold)
        }
    t = true;
    for(x = 0; x < img.width(); x++)
    {
        for(j = y; j < img.height(); j++)
            CMP(x, j, cropThreshold)
        }
    t = true;
    for(w = img.width(); w > 0; w--)
    {
        for(j = y; j < img.height(); j++)
            CMP(w - 1, j, cropThreshold)
        }
    t = true;
    for(h = img.height(); h > 0; h--)
    {
        for(j = x; j < w; j++)
            CMP(j, h - 1, cropThreshold)
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






