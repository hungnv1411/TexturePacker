#ifndef TPPROJECT_HPP
#define TPPROJECT_HPP

#include <QtCore>

struct TPConfigurations {
    bool isRecursive;
    QStringList folders;
    QStringList files;

    TPConfigurations() {
        isRecursive = true;
    }
};

class TPProject
{
public:
    TPProject();
    virtual ~TPProject();

    bool load(const QString& projectFilePath);
    void save(const QString& projectFilePath);

    TPConfigurations& getConfigurations() {
        return configs;
    }

    bool isModified() {
        return modified;
    }

private:
    void resolveRelativePath();

private:
    TPConfigurations configs;
    bool modified;
};

#endif // TPPROJECT_HPP
