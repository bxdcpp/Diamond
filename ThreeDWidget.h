#pragma once
#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>

class vtkImageData;
class vtkVolume;
class vtkRenderer;
class vtkOrientationMarkerWidget;
class ThreeDWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    ThreeDWidget(QWidget *parent = nullptr);
    ~ThreeDWidget();
    void SetImageData(vtkImageData* pImgData);
    vtkSmartPointer<vtkRenderer> GetRenderer();
	void ResetView();
	void SetVolumeVisible();
private:

    void RenderVolume();
    void AddAxes();
private:
    vtkSmartPointer<vtkImageData> m_pImageData = nullptr;
    vtkSmartPointer<vtkRenderer> m_pRenderer = nullptr;
    vtkSmartPointer<vtkOrientationMarkerWidget> m_pmarkWidget = nullptr;
	vtkSmartPointer<vtkVolume> m_volume = nullptr;
};
