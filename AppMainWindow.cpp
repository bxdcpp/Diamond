#include "AppMainWindow.h"
#include "SliceWidget.h"
#include "ThreeDWidget.h"
#include "vtkImageDataIO.h"

#include <QFileDialog>
#include <QStandardPaths>
#include <QToolButton>
#include <QDebug>
//#include <QAppDesktop>

#include <vtkImageData.h>
#include <vtkMatrix3x3.h>
#include <vtkImageChangeInformation.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

AppMainWindow::AppMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui = new Ui::AppMainWindow;
    ui->setupUi(this);
    InitMPRWidget();
    QObject::connect(ui->LoadBtn, SIGNAL(clicked(bool)), this, SLOT(onLoadData()));
    QObject::connect(ui->TestBtn, SIGNAL(clicked(bool)), this, SLOT(onTest()));
}

AppMainWindow::~AppMainWindow()
{
    delete ui;
}

void AppMainWindow::onTest()
{
    auto renderer = ui->sliceA->GetRenderer();
    auto camera = renderer->GetActiveCamera();
    double pos[3]{ 0 };
    camera->GetPosition(pos);
    qDebug() << "pos:" << QString("(%1,%2,%3)").arg(pos[0]).arg(pos[1]).arg(pos[2]);
    double fpoint[3]{ 0 };
    double viewUp[3]{ 0 };
    camera->GetFocalPoint(fpoint);
    camera->GetViewUp(viewUp);
    double s = camera->GetParallelScale();
    qDebug() << camera->GetViewAngle();
    qDebug() << "GetViewUp:" << QString("(%1,%2,%3)").arg(viewUp[0]).arg(viewUp[1]).arg(viewUp[2]);
    qDebug() << "GetFocalPoint:" << QString("(%1,%2,%3)").arg(fpoint[0]).arg(fpoint[1]).arg(fpoint[2]);
    qDebug() << "ParallelScale:" << QString("(%1)").arg(s);
    renderer->ResetCamera();

}

void AppMainWindow::InitMPRWidget()
{
   /* m_pSliceA = new SliceWidget(SliceWindowType::MPRA, ui->sliceA);
    m_pSliceC = new SliceWidget(SliceWindowType::MPRC, ui->sliceC);
    m_pSliceS = new SliceWidget(SliceWindowType::MPRS, ui->sliceS);
    ui->sliceA->layout()->addWidget(m_pSliceA);
    ui->sliceC->layout()->addWidget(m_pSliceC);
    ui->sliceS->layout()->addWidget(m_pSliceS);*/
    //QObject::connect(this,)
    ui->sliceA->SetType(MPRA);
    ui->sliceC->SetType(MPRC);
    ui->sliceS->SetType(MPRS);
}

void AppMainWindow::onLoadData()
{
    const QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    const QString path = QFileDialog::getOpenFileName(nullptr, QString::fromLocal8Bit("选择体数据文件"), defaultPath, "*.nii.gz");
    //QString path = QFileDialog::getOpenFileName(nullptr, tr("选择体数据文件"), defaultPath, "*.nii.gz");
    //QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
    //    "/home",
    //    tr("Images (*.png *.xpm *.jpg)"));
    vtkSmartPointer<vtkImageData> pImgData = vtkSmartPointer<vtkImageData>::New();
    if (vtkImageDataIO::readImageDataFromNiiGz(path.toStdString(), pImgData.Get()))
    {
        ui->sliceA->SetThreeDWidget(ui->threeDWidget);
        ui->sliceC->SetThreeDWidget(ui->threeDWidget);
        ui->sliceS->SetThreeDWidget(ui->threeDWidget);

        ui->sliceA->SetImageData(pImgData);
        ui->sliceC->SetImageData(pImgData);
        ui->sliceS->SetImageData(pImgData);
    
        //ui->threeDWidget->SetImageData(pImgData);
    }


   // vtkNew<vtkImageData> img;
   // img->DeepCopy(pImgData);
   ///* vtkNew<vtkMatrix3x3> mat;
   // mat->Identity();
   // img->SetDirectionMatrix(mat);
   // img->Modified();*/
   // vtkImageDataIO::saveImageDataToNiiGz("test_brain.nii.gz",img);
    //vtkNew<vtkImageChangeInformation> cinfo;
 
}
