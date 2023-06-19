#include "AppMainWindow.h"
//#include <QtWidgets/QApplication>
#include <QApplication>
#include <QSurfaceFormat>
#include <QVTKOpenGLNativeWidget.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Needed to ensure appropriate OpenGL context is created for VTK rendering.
    QSurfaceFormat::setDefaultFormat(QVTKOpenGLNativeWidget::defaultFormat());
    AppMainWindow w;
    w.show();
    return a.exec();
}
