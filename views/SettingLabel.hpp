#ifndef SETTINGLABEL_HPP
#define SETTINGLABEL_HPP

#include <QLabel>
#include <QPainter>

class SettingLabel : public QLabel
{
public:

    SettingLabel(const QString &text, QWidget *parent=Q_NULLPTR, Qt::WindowFlags f=Qt::WindowFlags());

    void paintEvent(QPaintEvent*)
    {
        QPainter painter(this);
        painter.translate(0,sizeHint().height());
        painter.rotate(270);

        painter.drawText(QRect (QPoint(0,0),QLabel::sizeHint()),Qt::AlignCenter,text());
    }
};

#endif // SETTINGLABEL_HPP
