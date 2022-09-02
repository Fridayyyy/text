//
// Created by 常贵杰 on 2022/8/23.
//

#include "text.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPainter>
#include <QColorDialog>
#include <QTextList>
#include <QTextListFormat>
#include <QActionGroup>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QTextBlockFormat>

Text::Text(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Text");

    showWidget = new ShowWidget(this);
    setCentralWidget(showWidget);


    fontLabel1 = new QLabel(tr("Font"));
    fontComboBox = new QFontComboBox;
    fontComboBox->setFontFilters(QFontComboBox::ScalableFonts);
    fontLabel2 = new QLabel(tr("Font Size"));
    sizeComboBox = new QComboBox;
    QFontDatabase db;
    foreach(int size,db.standardSizes())
        sizeComboBox->addItem(QString::number(size));
    boldBtn = new QToolButton;
    boldBtn->setCheckable(true);
    italicBtn = new QToolButton;
    italicBtn->setCheckable(true);
    underlineBtn = new QToolButton;
    underlineBtn->setCheckable(true);
    colorBtn = new QToolButton;
    colorBtn->setCheckable(true);



    //sort
    listLabel = new QLabel(tr("Sort"));
    listComboBox = new QComboBox;
    listComboBox->addItem("Standard");
    listComboBox->addItem("QTextListFormat::ListDisc");
    listComboBox->addItem("QTextListFormat::ListCircle");
    listComboBox->addItem("QTextListFormat::ListSquare");
    listComboBox->addItem("QTextListFormat::ListDecimal");
    listComboBox->addItem("QTextListFormat::ListLowerAlpha");
    listComboBox->addItem("QTextListFormat::ListUpperAlpha");
    listComboBox->addItem("QTextListFormat::ListLowerRoman");
    listComboBox->addItem("QTextListFormat::ListUpperRoman");


    createActions();
    createMenus();
    createToolBars();

    if(img.load("/Users/changguijie/CLionProjects/qt/text/image.png")){
        printf("1\n");
        showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
    }

    connect(showWidget->text,SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this,SLOT(ShowCurrentFormatChanged(const QTextCharFormat &)));
    connect(showWidget->text->document(),SIGNAL(undoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(showWidget->text->document(),SIGNAL(redoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(showWidget->text,SIGNAL(cursorPositionChanged()),this,SLOT(ShowCursorPositionChanged()));


    connect(fontComboBox, SIGNAL(activated(QString)), this, SLOT(ShowFontComboBox(QString)));
    connect(sizeComboBox, SIGNAL(activated(const QString &)), this, SLOT(ShowSizeSpinBox(QString)));
    connect(boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
    connect(italicBtn, SIGNAL(clicked()), this, SLOT(ShowUnderlineBtn()));
    connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));

    connect(listComboBox, SIGNAL(activated(int)), this, SLOT(ShowList(int)));

}
Text::~Text() {

}
void Text::createActions() {
    openFileAction = new QAction(tr("Open"),this);
    openFileAction->setStatusTip(tr("open the file"));
    connect(openFileAction,SIGNAL(triggered()),this,SLOT(ShowOpenFile()));

    newFileAction = new QAction(tr("New"),this);
    newFileAction->setStatusTip(tr("create new file"));
    connect(newFileAction,SIGNAL(triggered()),this,SLOT(ShowNewFile()));

    exitAction = new QAction(tr("Exit"),this);
    exitAction->setStatusTip(tr("exit this function"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    copyAction = new QAction(tr("Copy"),this);
    copyAction->setStatusTip("copy this file");

    cutAction = new QAction(QIcon("cut.png"), tr("Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut this file."));

    //粘贴 动作
    pasteAction = new QAction(QIcon("paste.png"), tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("paste the file."));

    //关于 动作
    aboutAction = new QAction(QIcon("about.png"), tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));


    //打印文本 动作
    printTextAction = new QAction(QIcon("printText.png"), tr("PrintText"), this);
    printTextAction->setStatusTip(tr("print a text."));
    connect(printTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));

    //打印图形 动作
    printImageAction = new QAction(QIcon("printImage.png"), tr("PrintImage"), this);
    printImageAction->setStatusTip(tr("print a Image."));
    connect(printImageAction, SIGNAL(triggered()), this, SLOT(ShowPrintImage()));

    //放大 动作
    zoomInaction = new QAction(QIcon("zoomin.png"), tr("ZoomIn"), this);
    zoomInaction->setStatusTip(tr("zoomin the image."));
    connect(zoomInaction, SIGNAL(triggered()), this, SLOT(ShowZoomIn()));

    //缩小 动作
    zoomOutAction = new QAction(QIcon("zoomout.png"), tr("ZoomOut"), this);
    zoomOutAction->setStatusTip(tr("zoomout the image."));
    connect(zoomOutAction, SIGNAL(triggered()), this, SLOT(ShowZoomOut()));

    //实现图像旋转动作
    //旋转90
    rotate90Action = new QAction(QIcon("rotate90.png"), tr("Rotate90"), this);
    rotate90Action->setStatusTip(tr("rotate the image 90."));
    connect(rotate90Action, SIGNAL(triggered()), this, SLOT(ShowRotate90()));

    //旋转180
    rotate180Action = new QAction(QIcon("rotate180.png"), tr("Rotate180"), this);
    rotate180Action->setStatusTip(tr("rotate the image 180."));
    connect(rotate180Action, SIGNAL(triggered()), this, SLOT(ShowRotate180()));

    //旋转270
    rotate270Action = new QAction(QIcon("rotate270.png"), tr("Rotate270"), this);
    rotate270Action->setStatusTip(tr("rotate the image 270."));
    connect(rotate270Action, SIGNAL(triggered()), this, SLOT(ShowRotate270()));

    //实现镜像动作
    //纵向
    mirrorVerticalAction = new QAction(QIcon("mirrorVertical.png"), tr("MirrorVertical"), this);
    mirrorVerticalAction->setStatusTip(tr("mirror vertical the image."));
    connect(mirrorVerticalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorVertical()));

    //横向
    mirrorHorizontalAction = new QAction(QIcon("mirrorHorizontal.png"), tr("MirrorHorizontal"), this);
    mirrorHorizontalAction->setStatusTip(tr("mirror Horizontal the image."));
    connect(mirrorHorizontalAction, SIGNAL(triggered()), this, SLOT(ShowMirrorHorizontal()));

    //实现撤销和恢复动作
    //撤销
    undoAction = new QAction(QIcon("undo.png"), tr("Undo"), this);

    //恢复
    redoAction = new QAction(QIcon("redo.png"), tr("Redo"), this);

    actGrp = new QActionGroup(this);
    leftAction = new QAction(tr("Left"),actGrp);
    leftAction->setCheckable(true);
    rightAction = new QAction(tr("Right"),actGrp);
    rightAction->setCheckable(true);
    centerAction = new QAction(tr("Center"),actGrp);
    centerAction->setCheckable(true);
    justifyAction = new QAction(tr("Justify"),actGrp);
    justifyAction->setCheckable(true);
    connect(actGrp,SIGNAL(triggered(QAction *)),this,SLOT(ShowAlignment(QAction *)));

}
void Text::createMenus() {
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(printTextAction);
    fileMenu->addAction(printImageAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    zoomMenu = menuBar()->addMenu(tr("Edit"));
    zoomMenu->addAction(copyAction);
    zoomMenu->addAction(cutAction);
    zoomMenu->addAction(pasteAction);
    zoomMenu->addAction(aboutAction);
    zoomMenu->addAction(zoomInaction);
    zoomMenu->addAction(zoomOutAction);

    rotateMenu = menuBar()->addMenu(tr("Rotate"));
    rotateMenu->addAction(rotate90Action);
    rotateMenu->addAction(rotate180Action);
    rotateMenu->addAction(rotate270Action);

    //镜像菜单
    mirrorMenu = menuBar()->addMenu(tr("Mirror"));
    mirrorMenu->addAction(mirrorVerticalAction);
    mirrorMenu->addAction(mirrorHorizontalAction);

}
void Text::createToolBars() {
    fileTool = addToolBar("File");
    fileTool->addAction(openFileAction);
    fileTool->addAction(newFileAction);
    fileTool->addAction(printTextAction);
    fileTool->addAction(printImageAction);

    zoomTool = addToolBar("Edit");
    zoomTool->addAction(copyAction);
    zoomTool->addAction(cutAction);
    zoomTool->addAction(pasteAction);
    zoomTool->addSeparator();
    zoomTool->addAction(zoomInaction);
    zoomTool->addAction(zoomOutAction);

    //旋转工具条
    rotateTool = addToolBar("Rotate");
    rotateTool->addAction(rotate90Action);
    rotateTool->addAction(rotate180Action);
    rotateTool->addAction(rotate270Action);

    //撤销和重做工具条
    doToolBar = addToolBar("doEdit");
    doToolBar->addAction(undoAction);
    doToolBar->addAction(redoAction);

    //font
    fontToolBar = addToolBar("Font");
    fontToolBar->addWidget(fontLabel1);
    fontToolBar->addWidget(fontComboBox);
    fontToolBar->addWidget(fontLabel2);
    fontToolBar->addWidget(sizeComboBox);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(boldBtn);
    fontToolBar->addWidget(italicBtn);
    fontToolBar->addWidget(underlineBtn);
    fontToolBar->addSeparator();
    fontToolBar->addWidget(colorBtn);

    listToolBar = addToolBar("List");
    listToolBar->addWidget(listLabel);
    listToolBar->addWidget(listComboBox);
    listToolBar->addSeparator();
    listToolBar->addActions(actGrp->actions());
}
void Text::ShowNewFile() {
    Text *newTextProcess = new Text;
    newTextProcess->show();
}
void Text::ShowAbout() {
    QMessageBox::aboutQt(this);
}
void Text::ShowOpenFile() {
    fileName = QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty()){
        if(showWidget->text->document()->isEmpty()){
            loadFile(fileName);
        } else{
            Text *newText = new Text;
            newText->show();
            newText->loadFile(fileName);
        }
    }
}
void Text::loadFile(QString fileName) {
    printf("file name:%s \n",fileName.data());
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream textStream(&file);
        while(!textStream.atEnd()){
            showWidget->text->append(textStream.readLine());
            printf("read line\n");
        }
        printf("read end\n");
    }
}
void Text::ShowPrintText() {
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec()){
        QTextDocument *doc=showWidget->text->document();
        doc->print(&printer);
    }
}
void Text::ShowPrintImage() {
    QPrinter printer;
    QPrintDialog printDialog(&printer,this);
    if(printDialog.exec()){
        QPainter painter(&printer);
        QRect rect=painter.viewport();
        QSize size = img.size();
        size.scale(rect.size(),Qt::KeepAspectRatio);
        painter.setViewport(rect.x(),rect.y(),size.width(),size.height());
        painter.setWindow(img.rect());
        painter.drawImage(0,0,img);
    }
}
void Text::ShowZoomIn() {
    if(img.isNull())
        return;
    //QMatrix4x4 matrix;
    //matrix.scale(2,2);
    img=img.scaled(2,2);
    showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}
void Text::ShowZoomOut() {
    if(img.isNull())
        return;
    //QMatrix4x4 matrix;
    //matrix.scale(0.5,0.5);
    img = img.scaled(0.5,0.5);
    showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}
void Text::ShowRotate90() {
    if(img.isNull())
        return;
    //QMatrix4x4 matrix;
    //matrix.rotate(90.0);
    //img=img.
    //showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}
void Text::ShowRotate180() {

}
void Text::ShowRotate270() {

}
void Text::ShowMirrorVertical() {
    if(img.isNull())
        return;
    img=img.mirrored(false,true);
    showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}
void Text::ShowMirrorHorizontal() {
    if(img.isNull())
        return;
    img=img.mirrored(true, false);
    showWidget->imageLabel->setPixmap(QPixmap::fromImage(img));
}
void Text::ShowFontComboBox(QString comboStr) {
    QTextCharFormat fmt;
    fmt.setFontFamily(comboStr);
    mergeFormat(fmt);
}
void Text::mergeFormat(QTextCharFormat format) {
    QTextCursor cursor = showWidget->text->textCursor();
    if (!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    showWidget->text->mergeCurrentCharFormat(format);
}
void Text::SHowSizeSpinBox(QString spinValue) {
    QTextCharFormat fmt;
    fmt.setFontPointSize(spinValue.toFloat());
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::ShowBoldBtn() {
    QTextCharFormat fmt;
    fmt.setFontWeight(boldBtn->isChecked()?QFont::Bold:QFont::Normal);
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::ShowItalicBtn() {
    QTextCharFormat fmt;
    fmt.setFontItalic(italicBtn->isChecked());
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::ShowUnderlineBtn() {
    QTextCharFormat fmt;
    fmt.setFontUnderline(underlineBtn->isChecked());
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::ShowColorBtn() {
    QColor color = QColorDialog::getColor(Qt::red,this);
    if(color.isValid()){
        QTextCharFormat fmt;
        fmt.setForeground(color);
        showWidget->text->mergeCurrentCharFormat(fmt);
    }
}

void Text::ShowCurrentFormatChanged(const QTextCharFormat &fmt) {
    fontComboBox->setCurrentIndex(fontComboBox->findText(fmt.fontFamily()));
    sizeComboBox->setCurrentIndex(sizeComboBox->findText(QString::number(fmt.fontPointSize())));
    boldBtn->setChecked(fmt.font().bold());
    italicBtn->setChecked(fmt.font().italic());
    underlineBtn->setChecked(fmt.fontUnderline());
}
void Text::ShowAlignment(QAction *act) {
    if (act==leftAction){
        showWidget->text->setAlignment(Qt::AlignLeft);
    }
    if(act == rightAction)
    {
        //右对齐
        showWidget->text->setAlignment(Qt::AlignRight);
    }
    if(act == centerAction)
    {
        //居中对齐
        showWidget->text->setAlignment(Qt::AlignCenter);
    }
    if(act == justifyAction)
    {
        //两端对齐
        showWidget->text->setAlignment(Qt::AlignJustify);
    }
}
void Text::ShowCursorPositionChanged() {
    if (showWidget->text->alignment()==Qt::AlignLeft){
        leftAction->setChecked(true);
    }
    if(showWidget->text->alignment() == Qt::AlignRight)
    {
        rightAction->setChecked(true);
    }
    if(showWidget->text->alignment() == Qt::AlignCenter)
    {
        centerAction->setChecked(true);
    }
    if(showWidget->text->alignment() == Qt::AlignJustify)
    {
        justifyAction->setChecked(true);
    }
}
void Text::ShowList(int index) {
    QTextCursor cursor = showWidget->text->textCursor();
    if(index!=0){
        QTextListFormat::Style style = QTextListFormat::ListDisc;
        switch (index){
            default:
            case 1:
                style = QTextListFormat::ListDisc;
                break;
            case 2:
                style = QTextListFormat::ListCircle;
                break;
            case 3:
                style = QTextListFormat::ListSquare;
                break;
            case 4:
                style = QTextListFormat::ListDecimal;
                break;
            case 5:
                style = QTextListFormat::ListLowerAlpha;
                break;
            case 6:
                style = QTextListFormat::ListUpperAlpha;
                break;
            case 7:
                style = QTextListFormat::ListLowerRoman;
                break;
            case 8:
                style = QTextListFormat::ListUpperRoman;
                break;
        }
        cursor.beginEditBlock();
        QTextBlockFormat blockFmt = cursor.blockFormat();
        QTextListFormat listFmt;
        if(cursor.currentList())
        {
            listFmt = cursor.currentList()->format();
        }
        else {
            listFmt.setIndent(blockFmt.indent() +1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
        cursor.endEditBlock();
    }
    else {
        QTextBlockFormat bfmt;
        bfmt.setObjectIndex(-1);
        cursor.mergeBlockFormat(bfmt);
    }

}

