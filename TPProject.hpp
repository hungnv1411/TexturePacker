#ifndef TPPROJECT_HPP
#define TPPROJECT_HPP

#include <QtCore>
#include "packer/Packer.hpp"

class TPProject
{
public:
    TPProject();
    virtual ~TPProject();

    bool load(const QString& projectFilePath);
    void save(const QString& projectFilePath);

    packer::Configurations& getConfigurations() {
        return configs;
    }

    bool isModified() {
        return modified;
    }

private:
    void resolveRelativePath();

private:
    packer::Configurations configs;
    bool modified;
};

#endif // TPPROJECT_HPP
