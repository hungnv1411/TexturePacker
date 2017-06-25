#ifndef TPPROJECT_HPP
#define TPPROJECT_HPP

#include <QtCore>

struct TPConfigurations {
    bool isSmartFolder;
    bool isRecursive;
    QString rootFolder;
    QStringList files;

    TPConfigurations() {
        isSmartFolder = false;
        isRecursive = true;
    }
};

class TPProject
{
public:
    TPProject();
    virtual ~TPProject();

    bool load(const QString& projectFilePath);
    void save();

    TPConfigurations& getConfigurations() {
        return configs;
    }

    bool isModified() {
        return modified;
    }

private:
    TPConfigurations configs;
    bool modified;
};

#endif // TPPROJECT_HPP
