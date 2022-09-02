#include <QApplication>
#include <QPushButton>
#include "text.h"
#include <QComboBox>
#include <QWidget>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Text t=new Text;
    t.show();

    return QApplication::exec();
}
