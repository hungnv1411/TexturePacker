#include "CocosExporter.hpp"
#include "../utils/Utils.hpp"

#include <QPainter>

namespace exporter {

CocosExporter::CocosExporter()
{

}

ExporterErrorCode CocosExporter::exportSpriteSheet(const packer::ImagePackerPtr& imagePacker, const ConfigurationsPtr& outputConfigs) {

    QString dataFilePath = QDir::toNativeSeparators(outputConfigs->dataFilePath);
    QString outDir = Utils::getContainingDirectory(dataFilePath);
    QString outFile = Utils::getFileName(dataFilePath);
    QString outFormat = outputConfigs->outputFormat;
    QString atlasExtension = Utils::getFileExtension(dataFilePath);

    packer::ConfigurationsPtr configs = imagePacker->getConfigurations();

    QList<QImage> textures;
    for(int i = 0; i < imagePacker->bins.size(); i++)
    {
        QImage texture(imagePacker->bins.at(i).width(), imagePacker->bins.at(i).height(), QImage::Format_ARGB32);
        texture.fill(Qt::transparent);
        textures << texture;
    }
    for (int j = 0; j < textures.count(); j++)
    {
        QString outputFile = outDir;
        outputFile += QDir::separator();
        outputFile += outFile;
        if(textures.count() > 1)
        {
            outputFile += QString("_") + QString::number(j + 1);
        }
        outputFile += "." + atlasExtension;
        QString imgFile = outFile;
        if(textures.count() > 1)
        {
            imgFile += QString("_") + QString::number(j + 1);
        }
        imgFile += ".";
        imgFile += outFormat.toLower();

        QFile positionsFile(outputFile);
        if(!positionsFile.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            return Export_Cannot_Create_File;
        }
        else
        {
            QTextStream out(&positionsFile);
            out << "textures: " << imgFile << "\n";
            for(int i = 0; i < imagePacker->sprites.size(); i++)
            {
                if(imagePacker->sprites.at(i).textureId != j)
                {
                    continue;
                }
                QPoint pos(imagePacker->sprites.at(i).pos.x() + configs->border.left + configs->extrude,
                           imagePacker->sprites.at(i).pos.y() + configs->border.top + configs->extrude);
                QSize size, sizeOrig;
                QRect crop;
                sizeOrig = imagePacker->sprites.at(i).size;
                if(!configs->cropThreshold)
                {
                    size = imagePacker->sprites.at(i).size;
                    crop = QRect(0, 0, size.width(), size.height());
                }
                else
                {
                    size = imagePacker->sprites.at(i).crop.size();
                    crop = imagePacker->sprites.at(i).crop;
                }
                if(imagePacker->sprites.at(i).rotated)
                {
                    size.transpose();
                    crop = QRect(crop.y(), crop.x(), crop.height(), crop.width());
                }
                out << Utils::getFileName(imagePacker->sprites[i].path)
                    <<
                    "\t" <<
                    pos.x() << "\t" <<
                    pos.y() << "\t" <<
                    crop.width() << "\t" <<
                    crop.height() << "\t" <<
                    crop.x() << "\t" <<
                    crop.y() << "\t" <<
                    sizeOrig.width() << "\t" <<
                    sizeOrig.height() << "\t" <<
                    (imagePacker->sprites.at(i).rotated ? "r" : "") << "\n";
            }
        }
    }
    for(int i = 0; i < imagePacker->sprites.size(); i++)
    {
        if(imagePacker->sprites.at(i).pos == QPoint(999999, 999999))
        {
            continue;
        }
        if(imagePacker->sprites.at(i).duplicatedSprite != NULL && configs->merge)
        {
            continue;
        }
        QPoint pos(imagePacker->sprites.at(i).pos.x() + configs->border.left, imagePacker->sprites.at(i).pos.y() + configs->border.top);
        QSize size;
        QRect crop;
        if(!configs->cropThreshold)
        {
            size = imagePacker->sprites.at(i).size;
            crop = QRect(0, 0, size.width(), size.height());
        }
        else
        {
            size = imagePacker->sprites.at(i).crop.size();
            crop = imagePacker->sprites.at(i).crop;
        }
        QImage img = imagePacker->sprites[i].texture;
        if(imagePacker->sprites.at(i).rotated)
        {
            QTransform myTransform;
            myTransform.rotate(90);
            img = img.transformed(myTransform);
            size.transpose();
            crop = QRect(imagePacker->sprites.at(i).size.height() - crop.y() - crop.height(),
                         crop.x(), crop.height(), crop.width());
        }
        if(imagePacker->sprites.at(i).textureId < imagePacker->bins.size())
        {
            QPainter p(&textures.operator [](imagePacker->sprites.at(i).textureId));
            if(configs->extrude)
            {
                QColor color1 = QColor::fromRgba(img.pixel(crop.x(), crop.y()));
                p.setPen(color1);
                p.setBrush(color1);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x(), pos.y()));
                }
                else
                {
                    p.drawRect(QRect(pos.x(), pos.y(), configs->extrude - 1, configs->extrude - 1));
                }

                QColor color2 = QColor::fromRgba(img.pixel(crop.x(), crop.y() + crop.height() - 1));
                p.setPen(color2);
                p.setBrush(color2);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x(), pos.y() + crop.height() + configs->extrude));
                }
                else
                {
                    p.drawRect(QRect(pos.x(), pos.y() + crop.height() + configs->extrude, configs->extrude - 1, configs->extrude - 1));
                }

                QColor color3 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                 crop.y()));
                p.setPen(color3);
                p.setBrush(color3);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x() + crop.width() + configs->extrude, pos.y()));
                }
                else
                {
                    p.drawRect(QRect(pos.x() + crop.width() + configs->extrude, pos.y(),
                                     configs->extrude - 1, configs->extrude - 1));
                }

                QColor color4 = QColor::fromRgba(img.pixel(crop.x() + crop.width() - 1,
                                                 crop.y() + crop.height() - 1));
                p.setPen(color4);
                p.setBrush(color4);
                if(configs->extrude == 1)
                {
                    p.drawPoint(QPoint(pos.x() + crop.width() + configs->extrude,
                                       pos.y() + crop.height() + configs->extrude));
                }
                else
                {
                    p.drawRect(QRect(pos.x() + crop.width() + configs->extrude,
                                     pos.y() + crop.height() + configs->extrude, configs->extrude - 1,
                                     configs->extrude - 1));
                }

                p.drawImage(QRect(pos.x(), pos.y() + configs->extrude, configs->extrude,
                                  crop.height()), img, QRect(crop.x(), crop.y(), 1, crop.height()));
                p.drawImage(QRect(pos.x() + crop.width() + configs->extrude,
                                  pos.y() + configs->extrude, configs->extrude, crop.height()), img,
                            QRect(crop.x() + crop.width() - 1, crop.y(), 1, crop.height()));

                p.drawImage(QRect(pos.x() + configs->extrude, pos.y(), crop.width(),
                                  configs->extrude), img, QRect(crop.x(), crop.y(), crop.width(), 1));
                p.drawImage(QRect(pos.x() + configs->extrude,
                                  pos.y() + crop.height() + configs->extrude, crop.width(), configs->extrude), img,
                            QRect(crop.x(), crop.y() + crop.height() - 1, crop.width(), 1));

                p.drawImage(pos.x() + configs->extrude, pos.y() + configs->extrude, img, crop.x(),
                            crop.y(), crop.width(), crop.height());
            }
            else
            {
                p.drawImage(pos.x(), pos.y(), img, crop.x(), crop.y(), crop.width(),
                            crop.height());
            }
        }
    }

    const char *format = qPrintable(outFormat);
    for(int i = 0; i < textures.count(); i++)
    {
        QString imgdirFile;
        imgdirFile = outDir;
        imgdirFile += QDir::separator();
        imgdirFile += outFile;
        if(textures.count() > 1)
        {
            imgdirFile += QString("_") + QString::number(i + 1);
        }
        imgdirFile += ".";
        imgdirFile += outFormat.toLower();
        if(outFormat == "JPG")
        {
            textures.at(i).save(imgdirFile, format, 100);
        }
        else
        {
            textures.at(i).save(imgdirFile);
        }
    }
    return Export_OK;
}

}
