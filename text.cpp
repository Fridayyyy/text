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
#include <iostream>

using namespace std;

Text::Text(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Text");

    showWidget = new ShowWidget(this);
    setCentralWidget(showWidget);


    fontLabel1 = new QLabel(tr("Font"));
    fontComboBox = new QFontComboBox;
    connect(fontComboBox, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(ShowFontComboBox( const QFont &)));


    fontLabel2 = new QLabel(tr("Font Size"));
    sizeComboBox = new QComboBox;
    sizeComboBox->addItem("8",8);
    sizeComboBox->addItem("12",12);
    sizeComboBox->addItem("16",16);
    sizeComboBox->addItem("24",24);
    sizeComboBox->addItem("36",36);
    connect(sizeComboBox, SIGNAL(activated(int)), this, SLOT(ShowSizeSpinBox(int)));


    //字体设置按钮
    boldBtn = new QPushButton("Bold",this);
    boldBtn->setCheckable(true);
    italicBtn = new QPushButton("Italic",this);
    italicBtn->setCheckable(true);
    underlineBtn = new QPushButton("Underline",this);
    underlineBtn->setCheckable(true);
    colorBtn = new QPushButton("Color",this);
    colorBtn->setCheckable(true);
    connect(boldBtn, SIGNAL(clicked()), this, SLOT(ShowBoldBtn()));
    connect(italicBtn, SIGNAL(clicked()), this, SLOT(ShowItalicBtn()));
    connect(underlineBtn,SIGNAL(clicked()),this,SLOT(ShowUnderlineBtn()));
    connect(colorBtn, SIGNAL(clicked()), this, SLOT(ShowColorBtn()));

    actGrp=new QActionGroup(this);
    leftAction=new QAction("左对齐",actGrp);
    leftAction->setCheckable(true);
    centerAction=new QAction("中对齐",actGrp);
    centerAction->setCheckable(true);
    rightAction=new QAction("右对齐",actGrp);
    rightAction->setCheckable(true);
    connect(actGrp,SIGNAL(triggered(QAction*)),this,SLOT(ShowAlignment(QAction*)));


    createActions();
    createMenus();
    createToolBars();

    connect(showWidget->text,SIGNAL(currentCharFormatChanged(const QTextCharFormat &)),
            this,SLOT(ShowCurrentFormatChanged(const QTextCharFormat &)));
    connect(showWidget->text->document(),SIGNAL(undoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(showWidget->text->document(),SIGNAL(redoAvailable(bool)),redoAction,SLOT(setEnabled(bool)));
    connect(showWidget->text,SIGNAL(cursorPositionChanged()),this,SLOT(ShowCursorPositionChanged()));

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

    saveFileAction=new QAction("Save",this);
    connect(saveFileAction,SIGNAL(triggered()),this,SLOT(SaveFile()));

    exitAction = new QAction(tr("Exit"),this);
    exitAction->setStatusTip(tr("exit this function"));
    connect(exitAction,SIGNAL(triggered()),this,SLOT(close()));

    copyAction = new QAction(tr("Copy"),this);
    copyAction->setStatusTip("copy this file");

    cutAction = new QAction( tr("Cut"), this);
    cutAction->setShortcut(tr("Ctrl+X"));
    cutAction->setStatusTip(tr("Cut this file."));

    //粘贴 动作
    pasteAction = new QAction(tr("Paste"), this);
    pasteAction->setShortcut(tr("Ctrl+V"));
    pasteAction->setStatusTip(tr("paste the file."));

    //关于 动作
    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(ShowAbout()));


    //打印文本 动作
    printTextAction = new QAction(tr("PrintText"), this);
    printTextAction->setStatusTip(tr("print a text."));
    connect(printTextAction, SIGNAL(triggered()), this, SLOT(ShowPrintText()));


    //实现撤销和恢复动作
    //撤销
    undoAction = new QAction(tr("Undo"), this);

    //恢复
    redoAction = new QAction(tr("Redo"), this);

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

    boldAction=new QAction("Bold",this);
    connect(boldAction, SIGNAL(triggered()),this,SLOT(textBold()));
    italicAction=new QAction("Italic",this);
    connect(italicAction,SIGNAL(triggered()),this,SLOT(textItalic()));
    underlineAction=new QAction("Underline",this);
    connect(underlineAction,SIGNAL(triggered()),this,SLOT(textUnderline()));
}

void Text::createMenus() {
    fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(openFileAction);
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(printTextAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    fontMenu=menuBar()->addMenu("Font");
    fontMenu->addAction(boldAction);
    fontMenu->addAction(italicAction);
    fontMenu->addAction(underlineAction);
}

void Text::createToolBars() {
    fileTool = addToolBar("File");
    fileTool->addAction(openFileAction);
    fileTool->addAction(newFileAction);
    fileTool->addAction(printTextAction);

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

    listToolBar= addToolBar("List");
    listToolBar->addActions(actGrp->actions());

}

void Text::textBold() {
    QTextCharFormat fmt;
    fmt.setFontWeight(QFont::Bold);
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::textItalic() {
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    showWidget->text->mergeCurrentCharFormat(fmt);
}
void Text::textUnderline() {
    QTextCharFormat fmt;
    fmt.setFontUnderline(true);
    showWidget->text->mergeCurrentCharFormat(fmt);
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

void Text::SaveFile() {
    QFile myfile("/Users/changguijie/CLionProjects/qt/text/test.text");
    if(myfile.open(QFile::WriteOnly|QFile::Truncate)){
        QTextStream out(&myfile);
        cout<<"--------"<<endl;
        out<<showWidget->text->toPlainText();
    }
}

void Text::mergeFormat(QTextCharFormat format) {
    QTextCursor cursor = showWidget->text->textCursor();
    if (!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
    }
    cursor.mergeCharFormat(format);
    showWidget->text->mergeCurrentCharFormat(format);
}

void Text::ShowFontComboBox(const QFont & font) {
    QTextCharFormat fmt;
    fmt.setFont(font);
    mergeFormat(fmt);
}
void Text::ShowSizeSpinBox(int size) {
    int s =sizeComboBox->itemData(size,Qt::UserRole).toInt();
    QTextCharFormat fmt;
    fmt.setFontPointSize(s);
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
    QTextCharFormat fmt;
    fmt.setForeground(color);
    showWidget->text->mergeCurrentCharFormat(fmt);
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