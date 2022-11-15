#include "SettingPanel.hpp"

SettingPanel::SettingPanel(QWidget *parent) : QWidget(parent)
{
    mainLayout = new QVBoxLayout();
//    mainLayout->setMargin(0);

    headerButton = new QPushButton(this);
    headerButton->setFixedHeight(30);

//    QPixmap pixmap(":/res/ic_setting_panel_header.png");
//    QPalette palette;
//    palette.setBrush(headerButton->backgroundRole(), QBrush(pixmap));
//    headerButton->setFlat(true);
//    headerButton->setAutoFillBackground(true);
//    headerButton->setPalette(palette);

//    headerButton->setStyleSheet("QPushButton { \
//                                    border-image: url(:/res/ic_setting_panel_header.png); \
//                                    font: 11px; \
//                                    text-align: left; \
//                                    padding-left: 10px; \
//                                }");

    mainLayout->addWidget(headerButton);
    mainLayout->setAlignment(headerButton, Qt::AlignTop);

    setLayout(mainLayout);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
