#include "SliceWidget.h"
#include "ColorLookupTable.h"
#include "ThreeDWidget.h"




//VTK includes
#include <vtkCylinderSource.h>
#include <vtkConeSource.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkImageMapToColors.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageMapper.h>
#include <vtkProperty2D.h>
#include <vtkMatrix4x4.h>
#include <vtkTransform.h>
#include <vtkCamera.h>
#include <vtkInteractorStyleImage.h>
#include <vtkImageActor.h>
#include <vtkImageMapper3D.h>
#include <vtkImageViewer2.h>
#include <vtkMatrix3x3.h>
#include <vtkPlaneSource.h>

class SliceImagePipeLine
{
public:
    SliceImagePipeLine(vtkImageData* pImageData,vtkMatrix4x4* pReSliceMatrix,vtkTransform* pTransform,double origin[3])
    {
        m_pImageReslice = vtkSmartPointer<vtkImageReslice>::New();
        m_pImageReslice->SetOutputDimensionality(2);
        m_pImageReslice->SetInterpolationModeToLinear();
        m_pImageReslice->SetBackgroundLevel(-1024);
        //m_pImageReslice->AutoCropOutputOn();
        m_pImageReslice->SetResliceAxes(pReSliceMatrix);
        m_pImageReslice->SetInputData(pImageData);
        //m_pImageReslice->SetOutputOrigin(origin);
       m_pImageReslice->SetResliceTransform(pTransform);

        m_pImageMapToWindowLevelColors = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
        m_pImageMapToWindowLevelColors->SetInputConnection(m_pImageReslice->GetOutputPort());
        m_pImageMapToWindowLevelColors->SetWindow(1000);
        m_pImageMapToWindowLevelColors->SetLevel(100);

        m_pImgMapToColors = vtkSmartPointer<vtkImageMapToColors>::New();
        vtkSmartPointer<vtkLookupTable> pLookupTable = vtkSmartPointer<vtkLookupTable>::New();
        pLookupTable->DeepCopy(ColorLookUpTable::GetTable(0));
        m_pImgMapToColors->SetLookupTable(pLookupTable);
        m_pImgMapToColors->SetInputConnection(m_pImageMapToWindowLevelColors->GetOutputPort());
        m_pImgMapToColors->SetOutputFormatToRGB();

       /* m_pImgMapper = vtkSmartPointer<vtkImageMapper>::New();
        m_pImgMapper->SetInputConnection(m_pImgMapToColors->GetOutputPort());
        m_pImageSliceActor = vtkSmartPointer<vtkActor2D>::New();
        m_pImageSliceActor->SetMapper(m_pImgMapper);*/
        //m_pImageSliceActor->GetProperty()->SetOpacity();
        vtkNew<vtkMatrix4x4> ResliceAxesAxes;
        double origin1[3]{ 0 };
        m_pImageReslice->GetOutputOrigin(origin1);
        ResliceAxesAxes->DeepCopy(m_pImageReslice->GetResliceAxes());
        m_pImageSliceActor = vtkSmartPointer<vtkImageActor>::New();
        m_pImageSliceActor->GetMapper()->SetInputConnection(m_pImgMapToColors->GetOutputPort());
        m_pImageSliceActor->SetUserMatrix(pReSliceMatrix);
        double pos[3]{ 0 };
        m_pImageSliceActor->GetPosition(pos);
        m_pImageSliceActor->GetMatrix()->Print(std::cout);
    }
    ~SliceImagePipeLine()
    {

    }
public:
    vtkSmartPointer<vtkImageReslice> m_pImageReslice = nullptr;
    vtkSmartPointer<vtkImageMapToWindowLevelColors> m_pImageMapToWindowLevelColors = nullptr;
    vtkSmartPointer<vtkImageMapToColors> m_pImgMapToColors = nullptr;
    vtkSmartPointer<vtkImageMapper> m_pImgMapper = nullptr;
    //vtkSmartPointer<vtkActor2D> m_pImageSliceActor = nullptr;
    vtkSmartPointer< vtkImageActor> m_pImageSliceActor = nullptr;



};

SliceWidget::SliceWidget(QWidget *parent)
    :QVTKOpenGLNativeWidget(parent)
{
    m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
    m_pRenderer->SetBackground(0, 0, 0);
    renderWindow()->AddRenderer(m_pRenderer);
    
    vtkSmartPointer<vtkInteractorStyleImage> pImageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindow()->GetInteractor()->SetInteractorStyle(pImageStyle);
    //vtkNew<vtkRenderWindowInteractor> WindowInteractor;
    //WindowInteractor->SetRenderWindow(renderWindow());
    //WindowInteractor->SetInteractorStyle(pImageStyle);
}

SliceWidget::~SliceWidget()
{
    renderWindow()->RemoveRenderer(m_pRenderer);
}

void SliceWidget::SetType(SliceWindowType type)
{
    m_eSliceType = type;
}

void SliceWidget::SetImageData(vtkImageData * pImageData)
{
    m_pImgData = pImageData;
    Test();
    InitReSliceMatrx();
}

void SliceWidget::SetThreeDWidget(ThreeDWidget* widget)
{
    m_pThreeDWidget = widget;

}

vtkSmartPointer<vtkRenderer> SliceWidget::GetRenderer()
{
    return m_pRenderer;
}

void SliceWidget::InitReSliceMatrx()
{
	/*double axialElements[16] = {
	   1, 0, 0, 0,
	   0, 1, 0, 0,
	   0, 0, 1, 0,
	   0, 0, 0, 1
	};
	double coronalElements[16] = {
		1, 0, 0, 0,
		0, 0, 1, 0,
		0,-1, 0, 0,
		0, 0, 0, 1 
    };

	double sagittalElements[16] = {
        0, 0,-1, 0,
        1, 0, 0, 0,
        0,-1, 0, 0,
        0, 0, 0, 1 
    };

	double obliqueElements[16] = {
	    1, 0, 0, 0,
	    0, 0.866025, -0.5, 0,
	    0, 0.5, 0.866025, 0,
	    0, 0, 0, 1 
    };*/


    //Öá×´¾ØÕó
   /* double axialElements[16] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };*/
    double axialElements[16] = {
     1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1
    };
    //Ê¸×´¾ØÕó
   /* double sagittalElements[16] = {
        0, 0, -1, 0,
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 1 };*/

    double sagittalElements[16] = {
    0, 0, 1, 0,
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 0, 1 };
    //¹Ú×´¾ØÕó
    double coronalElements[16] = {
        1, 0, 0, 0,
        0, 0, -1, 0,
        0, 1, 0, 0,
        0, 0, 0, 1 };

    if (m_presliceMatrix == nullptr)
        m_presliceMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
    if (m_presliceTransform == nullptr)
        m_presliceTransform = vtkSmartPointer<vtkTransform>::New();
    m_presliceTransform->Identity();
    m_presliceTransform->PostMultiply();
    int extent[6]{0};
    double spacing[3];
    double origin[3];
    double center[3]{ 0 };
    m_pImgData->GetExtent(extent);
    m_pImgData->GetSpacing(spacing);
    m_pImgData->GetOrigin(origin);
    m_pImgData->GetDirectionMatrix()->Print(std::cout);
    int centerIJK[3] = { extent[0] + extent[1] / 2, extent[2] + extent[3] / 2,extent[4] + extent[5] / 2 };

    std::cout << centerIJK[0] << " " << centerIJK[1] << " " << centerIJK[2] << " " << std::endl;
    m_pImgData->TransformIndexToPhysicalPoint(centerIJK, center);
    std::cout << center[0] << " " << center[1] << " " << center[2] << " " << std::endl;;
    double center2[3];
    m_pImgData->GetCenter(center2);
    std::cout << center2[0] << " " << center2[1] << " "<<center2[2] << " " << std::endl;;

    center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
    center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
    center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

    double yd = (extent[3] - extent[2] + 1) * spacing[1];

    std::cout << center[0] << " " << center[1] << " " << center[2] << " " << std::endl;
    double viewup[3]{ 0,1,0 };
    double viewforward[4]{ 0,0,1,1 };



    vtkNew<vtkMatrix4x4> direction;
    vtkMatrix3x3* m3 = m_pImgData->GetDirectionMatrix();
    vtkNew<vtkTransform> sliceTransform;
    sliceTransform->Identity();

    direction->SetElement(0, 0, m3->GetElement(0, 0));
    direction->SetElement(0, 1, m3->GetElement(0, 1));
    direction->SetElement(0, 2, m3->GetElement(0, 2));
    direction->SetElement(1, 0, m3->GetElement(1, 0));
    direction->SetElement(1, 1, m3->GetElement(1, 1));
    direction->SetElement(1, 2, m3->GetElement(1, 2));
    direction->SetElement(2, 0, m3->GetElement(2, 0));
    direction->SetElement(2, 1, m3->GetElement(2, 1));
    direction->SetElement(2, 2, m3->GetElement(2, 2));
    direction->Invert();

    sliceTransform->Translate(origin);
    sliceTransform->Concatenate(direction);
    sliceTransform->Translate(-origin[0], -origin[1], -origin[2]);
    sliceTransform->Update();




    vtkMatrix3x3* pDirMatrx= m_pImgData->GetDirectionMatrix();
    vtkNew<vtkMatrix4x4> dirMatrx4;
    dirMatrx4->Identity();

    vtkNew<vtkMatrix4x4> ras2lps;
    ras2lps->Identity();
    //ras2lps->SetElement(1, 1, -1);

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            dirMatrx4->SetElement(i, j, pDirMatrx->GetElement(i, j));
        }
        //dirMatrx4->SetElement(i, 3, center[i]);
    }
   //dirMatrx4->Invert();
    
   //ResetResliceMatrix();
    switch (m_eSliceType)
    {
    case Empty:
        break;
    case VR3D1:
        break;
    case MPRA:
        m_presliceMatrix->DeepCopy(axialElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Translate(center[0], center[1], center[2]);
        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());

        break;
    case MPRC:
        m_presliceMatrix->DeepCopy(coronalElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Translate(center[0], center[1], center[2]);
     
        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());
    
        break;
    case MPRS:
        m_presliceMatrix->DeepCopy(sagittalElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Translate(center[0], center[1], center[2]);

        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());

        break;
    case CPR1:
        break;
    case VR3D2:
        break;
    default:
        break;
    }

  /* vtkNew<vtkMatrix4x4> Phys2index;
    Phys2index->DeepCopy(m_pImgData->GetPhysicalToIndexMatrix());
    vtkMatrix4x4::Multiply4x4(m_presliceMatrix, Phys2index, m_presliceMatrix);*/

    if (!m_pSliceImagePipeLine)
    {
        m_pSliceImagePipeLine = std::make_shared<SliceImagePipeLine>(m_pImgData,m_presliceMatrix, sliceTransform,center);
    }


    m_pRenderer->AddActor(m_pSliceImagePipeLine->m_pImageSliceActor);
   /* if (m_pThreeDWidget)
    {
        m_pThreeDWidget->GetRenderer()->AddActor(m_pSliceImagePipeLine->m_pImageSliceActor);
        m_pThreeDWidget->GetRenderer()->ResetCamera();
        m_pThreeDWidget->renderWindow()->Render();
    }*/
    

    /*viewforward[0] = m_presliceMatrix->Element[0][2];
    viewforward[1] = m_presliceMatrix->Element[1][2];
    viewforward[2] = m_presliceMatrix->Element[2][2];
    viewup[0] = m_presliceMatrix->Element[0][1];
    viewup[1] = m_presliceMatrix->Element[1][1];
    viewup[2] = m_presliceMatrix->Element[2][1];

    double distance = m_pRenderer->GetActiveCamera()->GetDistance();
    double focalPoint[3];
    m_pRenderer->GetActiveCamera()->GetFocalPoint(focalPoint);
    double cameraPositon[3]{ focalPoint[0] + viewforward[0] * distance,
                            focalPoint[1] + viewforward[1] * distance,
                            focalPoint[2] + viewforward[2] * distance };
    m_pRenderer->GetActiveCamera()->SetPosition(cameraPositon);
    m_pRenderer->GetActiveCamera()->SetViewUp(viewup);*/

    


    auto camera = m_pRenderer->GetActiveCamera();

    camera->ParallelProjectionOn();
    camera->SetViewAngle(30.0);

    double distance = camera->GetDistance();
    distance = 1;
    double focalPoint[3];
    //camera->GetParallelProjection();
    camera->GetFocalPoint(focalPoint);
    camera->SetParallelScale(0.5f * yd);
    
    double cameraPositon[3]{ center[0] + 0 * distance,
                          center[1] + 0 * distance,
                          center[2]  -1 * distance };
    camera->SetPosition(cameraPositon);
    camera->SetFocalPoint(center);
    camera->SetViewUp(0, -1, 0);
    //camera->SetParallelScale(0.5);

  
    m_presliceMatrix->Print(std::cout);
   /* m_pRenderer->ComputeVisiblePropBounds();
    m_pRenderer->ResetCameraClippingRange();*/

    //m_pRenderer->ResetCamera();
    m_pRenderer->Render();
    renderWindow()->Render();

}



void SliceWidget::ResetResliceMatrix()
{
    double center[3]{ 0 };

    vtkNew<vtkMatrix4x4> mdir;
    mdir->Identity();


	vtkImageData* image = m_pImgData;
	vtkMatrix3x3* m3 = image->GetDirectionMatrix();
	int extent[6];
	image->GetExtent(extent);
	int centerIJK[3] = { extent[0] + extent[1] / 2, extent[2] + extent[3] / 2,extent[4] + extent[5] / 2 };
	image->TransformIndexToPhysicalPoint(centerIJK, center);

	double center2[3];
	image->GetCenter(center2);

	double max = 0;
	int index = 0;
	int multi = 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			double v = m3->GetElement(i, j);
			if (std::abs(v) > max)
			{
				max = std::abs(v);
				index = j;
				if (v > 0)
				{
					multi = 1;
				}
				else
				{
					multi = -1;
				}
			}
		}
		mdir->SetElement(i, 0, m3->GetElement(index, 0) * multi);
		mdir->SetElement(i, 1, m3->GetElement(index, 1) * multi);
		mdir->SetElement(i, 2, m3->GetElement(index, 2) * multi);
		max = 0;
		index = 0;
		multi = 1;
	}

	m_presliceTransform->Identity();
	m_presliceTransform->PostMultiply();
    double viewUp[3], viewNormal[3];
    switch (m_eSliceType)
    {
        case MPRA:
        {
            viewUp[0] = 0;
            viewUp[1] = -1;
            viewUp[2] = 0;

            viewNormal[0] = 0;
            viewNormal[1] = 0;
            viewNormal[2] = -1;
            break;
        }

        case MPRS:
        {
            viewUp[0] = 0;
            viewUp[1] = 0;
            viewUp[2] = 1;

            viewNormal[0] = 1;
            viewNormal[1] = 0;
            viewNormal[2] = 0;

            m_presliceTransform->RotateZ(90);
            m_presliceTransform->RotateY(-90);
            break;
        }

        case MPRC:
        {
            viewUp[0] = 0;
            viewUp[1] = 0;
            viewUp[2] = 1;

            viewNormal[0] = 0;
            viewNormal[1] = -1;
            viewNormal[2] = 0;

            m_presliceTransform->RotateX(-90);
            break;
        }
    }
	m_presliceTransform->Concatenate(mdir);
	m_presliceTransform->Translate(center[0], center[1], center[2]);

	m_presliceTransform->Update();
	m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());
	m_center[0] = m_presliceMatrix->Element[0][3];
	m_center[1] = m_presliceMatrix->Element[1][3];
	m_center[2] = m_presliceMatrix->Element[2][3];

	m_viewUp[0] = m_presliceMatrix->Element[0][1];
	m_viewUp[1] = m_presliceMatrix->Element[1][1];
	m_viewUp[2] = m_presliceMatrix->Element[2][1];

	m_viewNormal[0] = m_presliceMatrix->Element[0][2];
	m_viewNormal[1] = m_presliceMatrix->Element[1][2];
	m_viewNormal[2] = m_presliceMatrix->Element[2][2];

	if (vtkMath::Dot(m_viewUp, viewUp) < 0)
	{
		vtkMath::MultiplyScalar(m_viewUp, -1);
	}

	if (vtkMath::Dot(m_viewNormal, viewNormal) < 0)
	{
		vtkMath::MultiplyScalar(m_viewNormal, -1);
	}

	if (m_eSliceType == MPRA || m_eSliceType == MPRC)
	{
		m_presliceMatrix->SetElement(0, 2, -m_viewNormal[0]);
		m_presliceMatrix->SetElement(1, 2, -m_viewNormal[1]);
		m_presliceMatrix->SetElement(2, 2, -m_viewNormal[2]);
	}
	else
	{
		m_presliceMatrix->SetElement(0, 2, m_viewNormal[0]);
		m_presliceMatrix->SetElement(1, 2, m_viewNormal[1]);
		m_presliceMatrix->SetElement(2, 2, m_viewNormal[2]);
	}

	m_presliceTransform->SetMatrix(m_presliceMatrix);
    
}

void SliceWidget::Test()
{
    vtkNew<vtkTransform> sliceTransform;
    sliceTransform->Identity();
    auto image = m_pImgData;

    if (image != nullptr)
    {
        double origin[3];
        image->GetOrigin(origin);

        vtkNew<vtkMatrix4x4> direction;
        vtkMatrix3x3* m3 = image->GetDirectionMatrix();

        direction->SetElement(0, 0, m3->GetElement(0, 0));
        direction->SetElement(0, 1, m3->GetElement(0, 1));
        direction->SetElement(0, 2, m3->GetElement(0, 2));
        direction->SetElement(1, 0, m3->GetElement(1, 0));
        direction->SetElement(1, 1, m3->GetElement(1, 1));
        direction->SetElement(1, 2, m3->GetElement(1, 2));
        direction->SetElement(2, 0, m3->GetElement(2, 0));
        direction->SetElement(2, 1, m3->GetElement(2, 1));
        direction->SetElement(2, 2, m3->GetElement(2, 2));
        direction->Invert();

        sliceTransform->Translate(origin);
        sliceTransform->Concatenate(direction);
        sliceTransform->Translate(-origin[0], -origin[1], -origin[2]);
        sliceTransform->Update();
        sliceTransform->GetMatrix()->Print(std::cout);

        vtkNew<vtkMatrix4x4> dirMatrx4;
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                dirMatrx4->SetElement(i, j, m3->GetElement(i, j));
            }
            dirMatrx4->SetElement(i, 3, origin[i]);
        }
        dirMatrx4->Invert();

        dirMatrx4->Print(std::cout);
    }
    image->GetPhysicalToIndexMatrix()->Print(std::cout);
}
