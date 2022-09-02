//
// Created by 常贵杰 on 2022/8/23.
//

#ifndef TEXT_SHOWWIDGET_H
#define TEXT_SHOWWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QTextEdit>
#include <QImage>

class ShowWidget: public QWidget{
    Q_OBJECT
public:
    explicit ShowWidget(QWidget *parent = 0);
    QImage img;//图像
    QLabel *imageLabel;//显示图像的控件
    QTextEdit *text;//文本数据

signals:

public slots:

};


#endif //TEXT_SHOWWIDGET_H
