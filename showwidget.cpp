//
// Created by 常贵杰 on 2022/8/23.
//

#include "showwidget.h"
#include <QHBoxLayout>

ShowWidget::ShowWidget(QWidget *parent) {
    text = new QTextEdit;

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->addWidget(text);
}