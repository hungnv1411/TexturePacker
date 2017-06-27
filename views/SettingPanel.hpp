#ifndef SETTINGPANEL_HPP
#define SETTINGPANEL_HPP

#include <QtWidgets>

class SettingPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SettingPanel(QWidget *parent = nullptr);

signals:

public slots:

protected:
    QPushButton* headerButton;
    QVBoxLayout* mainLayout;
};

#endif // SETTINGPANEL_HPP
