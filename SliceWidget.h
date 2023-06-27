#pragma once
#include "CommonDef.h"
   
#include <QVTKOpenGLNativeWidget.h>
#include <vtkActor2D.h>
#include <vtkSmartPointer.h>

class ThreeDWidget;
class vtkRenderer;
class vtkImageData;
class vtkMatrix4x4;
class vtkTransform;
class SliceImagePipeLine;
class SliceImagePipeLine2;
class SliceWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT

public:
    SliceWidget(QWidget *parent = nullptr);
    ~SliceWidget();
    void SetType(SliceWindowType type);
    void SetImageData(vtkImageData* pImageData);
    void SetThreeDWidget(ThreeDWidget* widget);
    vtkSmartPointer<vtkRenderer> GetRenderer();

private:
    void InitReSliceMatrx();
    void ResetResliceMatrix();
    void Test();
	void InitCamera(double center[3], double scale,vtkMatrix4x4* dirMat);
	void InitCamera2(double center[3], double scale);
	void GetSliceToWorldOrientation(vtkMatrix4x4* sliceToWorld);
	void InitMRHeadCamera(double center[3], double scale);
	void AddSphere(vtkRenderer* renderer, double pos[3],double c[3],vtkMatrix4x4* userM4 = nullptr);
private:
    SliceWindowType m_eSliceType = Empty;
    //vtkSmartPointer<vtkActor2D> m_pActor2D = nullptr;
    vtkSmartPointer<vtkRenderer> m_pRenderer = nullptr;
    vtkSmartPointer<vtkImageData> m_pImgData = nullptr;
    vtkSmartPointer<vtkMatrix4x4> m_presliceMatrix = nullptr;
    vtkSmartPointer<vtkTransform> m_presliceTransform = nullptr;
    std::shared_ptr<SliceImagePipeLine> m_pSliceImagePipeLine = nullptr;
    ThreeDWidget* m_pThreeDWidget = nullptr;


    double m_center[3];
    double m_viewUp[3];
    double m_viewNormal[3];
};
