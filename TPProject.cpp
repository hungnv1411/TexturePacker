#include "TPProject.hpp"

TPProject::TPProject()
    : modified(false) {
    configs = packer::ConfigurationsPtr(new packer::Configurations());
}

TPProject::~TPProject() {

}

bool TPProject::load(const QString &projectFilePath) {
    return true;
}

void TPProject::save(const QString &projectFilePath) {

}

void TPProject::resolveRelativePath() {

}








