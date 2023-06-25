#include "ThreeDWidget.h"
#include "vtkImageDataIO.h"
//VTK includes
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkNIFTIImageReader.h>
#include <vtkImageCast.h>
#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkVolumeProperty.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkAxesActor.h>


#include <QDebug>

//QT include 

ThreeDWidget::ThreeDWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    //read all the dicom files with the received path.
    /*vtkSmartPointer<vtkImageData> img = vtkSmartPointer<vtkImageData>::New();
    vtkImageDataIO::readImageDataFromNiiGz(std::string("S:/2_CT.nii.gz"), img);*/
    m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow()->AddRenderer(m_pRenderer);
    AddAxes();
}

ThreeDWidget::~ThreeDWidget()
{}

void ThreeDWidget::SetImageData(vtkImageData * pImgData)
{
    m_pImageData = pImgData;
    if (m_pImageData)
    {
        qDebug() << "get ImageData";
      /*  m_pImageData->Print(std::cout);*/
    }
        
    RenderVolume();
}

vtkSmartPointer<vtkRenderer> ThreeDWidget::GetRenderer()
{
    return m_pRenderer;
}

void ThreeDWidget::RenderVolume()
{
    vtkSmartPointer<vtkImageCast> cast = vtkSmartPointer<vtkImageCast>::New();
    cast->SetInputData(m_pImageData);
    //cast->SetOutputScalarTypeToFloat();
    cast->Update();

    //**********************************************************************
    vtkSmartPointer<vtkGPUVolumeRayCastMapper> volumeMapperGpu = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    //基于GPU加速的光线投射体绘制算法
    volumeMapperGpu->SetInputData(cast->GetOutput());
    volumeMapperGpu->SetImageSampleDistance(1.);  //0.5 ==> 1.0
    volumeMapperGpu->SetSampleDistance(1.0);
    volumeMapperGpu->SetAutoAdjustSampleDistances(1);

    //***********************************************************************
    vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();//定义对象属性
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetAmbient(0.4); //0.4
    volumeProperty->SetDiffuse(0.5);
    volumeProperty->SetSpecular(0.1);

    vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity = vtkSmartPointer<vtkPiecewiseFunction>::New();
    //Defines a piecewise function mapping.
   /* compositeOpacity->AddPoint(60, 0.00);
    compositeOpacity->AddPoint(140, 0.40);
    compositeOpacity->AddPoint(180, 0.90);*/

    compositeOpacity->AddPoint(0, 0.00);
    compositeOpacity->AddPoint(1024, 1.0);


    volumeProperty->SetScalarOpacity(compositeOpacity);

    vtkSmartPointer<vtkColorTransferFunction> color = vtkSmartPointer<vtkColorTransferFunction>::New();
    color->AddRGBPoint(0, 0, 0, 0);
    color->AddRGBPoint(64, 1, 0.52, 0.30);
    color->AddRGBPoint(190.0, 1, 1, 1);
    color->AddRGBPoint(220, 0.2, 0.2, 0.2);

    //volumeProperty->SetColor(color);

    vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
    //represents a volume(data & properties) in a rendered scene
    volume->SetMapper(volumeMapperGpu);
    volume->SetProperty(volumeProperty);
   

    m_pRenderer->AddVolume(volume);
    renderWindow()->Render();
}

void ThreeDWidget::AddAxes()
{
    // 显示坐标系的vtk组件 
    vtkSmartPointer<vtkAxesActor> axes_actor = vtkSmartPointer<vtkAxesActor>::New();
    axes_actor->SetPosition(0, 0, 0);
    axes_actor->SetTotalLength(2, 2, 2);
    axes_actor->SetShaftType(0);
    axes_actor->SetCylinderRadius(0.02);

    // 控制坐标系，使之随视角共同变化 
    m_pmarkWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
    m_pmarkWidget->SetOrientationMarker(axes_actor);

    m_pmarkWidget->SetInteractor(renderWindow()->GetInteractor());
    m_pmarkWidget->On();
    renderWindow()->Render();
}
