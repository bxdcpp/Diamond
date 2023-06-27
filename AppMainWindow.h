#pragma once

#include <QMainWindow>
#include "ui_AppMainWindow.h"

class SliceWidget;
class AppMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    AppMainWindow(QWidget *parent = nullptr);
    ~AppMainWindow();

protected slots:
    void onLoadData();
    void onTest();
	void onVisible();

private:
    void InitMPRWidget();

private:
   
    Ui::AppMainWindow* ui = nullptr;
    SliceWidget* m_pSliceA = nullptr;
    SliceWidget* m_pSliceC = nullptr;
    SliceWidget* m_pSliceS = nullptr;
};
