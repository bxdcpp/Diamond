#pragma once
#include "CommonDef.h"

#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor2D.h>
#include <vtkSmartPointer.h>

class vtkRenderer;
class vtkImageData;
class vtkMatrix4x4;
class vtkTransform;
class SliceImagePipeLine;
class SliceWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    SliceWidget(QWidget *parent = nullptr);
    ~SliceWidget();
    void SetType(SliceWindowType type);
    void SetImageData(vtkImageData* pImageData);

private:
    void InitReSliceMatrx();
    void RenderCone();
private:
    SliceWindowType m_eSliceType = Empty;
    //vtkSmartPointer<vtkActor2D> m_pActor2D = nullptr;
    vtkSmartPointer<vtkRenderer> m_pRenderer = nullptr;
    vtkSmartPointer<vtkImageData> m_pImgData = nullptr;
    vtkSmartPointer<vtkMatrix4x4> m_presliceMatrix = nullptr;
    vtkSmartPointer<vtkTransform> m_presliceTransform = nullptr;
    std::shared_ptr<SliceImagePipeLine> m_pSliceImagePipeLine = nullptr;
};