//
// Created by 常贵杰 on 2022/8/23.
//

#ifndef TEXT_TEXT_H
#define TEXT_TEXT_H
#include <QMainWindow>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QSpinBox>
#include <QToolBar>
#include <QFontComboBox>
#include <QToolButton>
#include <QTextCharFormat>
#include <QApplication>
#include <QPushButton>
#include "showwidget.h"

class Text : public QMainWindow{

    Q_OBJECT
public:
    Text(QWidget *parent = 0);
    ~Text();

    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString  fileName);
    void mergeFormat(QTextCharFormat format);

protected slots:
    void ShowNewFile();
    void ShowOpenFile();
    void SaveFile();
    void ShowPrintText();
    void ShowAbout();

    void textBold();
    void textItalic();
    void textUnderline();


    void ShowFontComboBox(const QFont &);//字体类型
    void ShowSizeSpinBox(int size);//字体大小
    void ShowBoldBtn();
    void ShowItalicBtn();
    void ShowUnderlineBtn();
    void ShowColorBtn();
    void ShowCurrentFormatChanged(const QTextCharFormat &fmt);


    void ShowAlignment(QAction *act);
    void ShowCursorPositionChanged();

private:
    QImage img;
    QString fileName;

    QMenu *fileMenu;
    QMenu *fontMenu;

    QAction *openFileAction;
    QAction *newFileAction;
    QAction *saveFileAction;

    QAction *printTextAction;
    QAction *printImageAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *aboutAction;

    QAction *boldAction;
    QAction *italicAction;
    QAction *underlineAction;

    QAction *undoAction;    //撤销动作控件
    QAction *redoAction;

    //toolbar
    QToolBar *fileTool;
    QToolBar *doToolBar;

    //button and label
    QLabel *fontLabel1;
    QFontComboBox *fontComboBox;
    QLabel *fontLabel2;
    QComboBox *sizeComboBox;
    QPushButton *boldBtn;
    QPushButton *italicBtn;
    QPushButton *underlineBtn;
    QPushButton *colorBtn;
    QToolBar *fontToolBar;

    //文本对齐
    QLabel *listLabel;
    QComboBox *listComboBox;
    QActionGroup *actGrp;
    QAction *leftAction;
    QAction *rightAction;
    QAction *centerAction;
    QAction *justifyAction;
    QToolBar *listToolBar;

    ShowWidget *showWidget;

};


#endif //TEXT_TEXT_H
