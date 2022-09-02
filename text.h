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
    void ShowPrintText();
    void ShowPrintImage();
    void ShowZoomIn();
    void ShowZoomOut();
    void ShowRotate90();
    void ShowRotate180();
    void ShowRotate270();
    void ShowMirrorVertical();
    void ShowMirrorHorizontal();
    void ShowAbout();

    void ShowFontComboBox(QString comboStr);//字体类型
    void SHowSizeSpinBox(QString spinValue);//字体大小
    void ShowBoldBtn();
    void ShowItalicBtn();
    void ShowUnderlineBtn();
    void ShowColorBtn();
    void ShowCurrentFormatChanged(const QTextCharFormat &fmt);

    void ShowList(int);
    void ShowAlignment(QAction *act);
    void ShowCursorPositionChanged();

private:
    QImage img;
    QString fileName;

    QMenu *fileMenu;
    QMenu *zoomMenu;
    QMenu *rotateMenu;
    QMenu *mirrorMenu;

    QAction *openFileAction;
    QAction *newFileAction;
    QAction *printTextAction;
    QAction *printImageAction;
    QAction *exitAction;
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;
    QAction *aboutAction;
    QAction *zoomInaction;
    QAction *zoomOutAction;    //缩小动作控件
    QAction *rotate90Action;    //旋转90度动作控件
    QAction *rotate180Action;    //旋转180度动作控件
    QAction *rotate270Action;    //旋转270度动作控件
    QAction *mirrorVerticalAction;    //横向镜像动作控件
    QAction *mirrorHorizontalAction;    //垂直镜像动作控件
    QAction *undoAction;    //撤销动作控件
    QAction *redoAction;

    //toolbar
    QToolBar *fileTool;
    QToolBar *zoomTool;
    QToolBar *rotateTool;
    QToolBar *mirrorTool;
    QToolBar *doToolBar;

    //button and label
    QLabel *fontLabel1;
    QFontComboBox *fontComboBox;
    QLabel *fontLabel2;
    QComboBox *sizeComboBox;
    QToolButton *boldBtn;
    QToolButton *italicBtn;
    QToolButton *underlineBtn;
    QToolButton *colorBtn;
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
