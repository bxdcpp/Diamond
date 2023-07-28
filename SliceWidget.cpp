#include "SliceWidget.h"
#include "ColorLookupTable.h"
#include "ThreeDWidget.h"


//QT include 
#include <QMouseEvent>


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
#include <vtkSphereSource.h>

class SliceImagePipeLine
{
public:
    SliceImagePipeLine(vtkImageData* pImageData,vtkMatrix4x4* pReSliceMatrix,vtkTransform* pTransform,double origin[3])
    {
        m_pImageReslice = vtkSmartPointer<vtkImageReslice>::New();
        m_pImageReslice->SetOutputDimensionality(2);
        m_pImageReslice->SetInterpolationModeToLinear();
        m_pImageReslice->SetBackgroundLevel(0);
        m_pImageReslice->AutoCropOutputOn();
        m_pImageReslice->SetResliceAxes(pReSliceMatrix);
        m_pImageReslice->SetInputData(pImageData);
        //m_pImageReslice->SetOutputOrigin(origin);
       //m_pImageReslice->SetResliceTransform(pTransform);

        m_pImageMapToWindowLevelColors = vtkSmartPointer<vtkImageMapToWindowLevelColors>::New();
        m_pImageMapToWindowLevelColors->SetInputConnection(m_pImageReslice->GetOutputPort());
        m_pImageMapToWindowLevelColors->SetWindow(1000);
        m_pImageMapToWindowLevelColors->SetLevel(360);

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
		actorImageIn3D = vtkSmartPointer< vtkImageActor>::New();
		actorImageIn3D->GetMapper()->SetInputConnection(m_pImgMapToColors->GetOutputPort());
		actorImageIn3D->SetUserMatrix(pReSliceMatrix);
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
	vtkSmartPointer< vtkImageActor> actorImageIn3D = nullptr;

};

SliceWidget::SliceWidget(QWidget *parent)
    :QVTKOpenGLNativeWidget(parent)
{
    m_pRenderer = vtkSmartPointer<vtkRenderer>::New();
    //m_pRenderer->SetBackground(135/255.0, 206/255.0, 235/255.0);
	m_pRenderer->SetBackground(0, 0.0, 0.0);
    renderWindow()->AddRenderer(m_pRenderer);
    
    /*vtkSmartPointer<vtkInteractorStyleImage> pImageStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindow()->GetInteractor()->SetInteractorStyle(pImageStyle);*/
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
    //Test();
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

void SliceWidget::mouseMoveEvent(QMouseEvent * event)
{
}

void SliceWidget::mousePressEvent(QMouseEvent * event)
{
	

}

void SliceWidget::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() == Qt::RightButton)
	{
		double worldPos[3]{0};
		int displayPosition[2]{ 0 };
		this->renderWindow()->GetInteractor()->GetEventPosition(displayPosition);
		double devicePos[3]{ displayPosition[0] ,displayPosition[1] ,1 };
		DisplayToWorld(displayPosition, worldPos);
		//XYToLPS(devicePos, worldPos);
		double c[3]{ 0,1,0 };
		double c2[3]{ 1,0,0 };
		AddSphere(m_pThreeDWidget->GetRenderer(), worldPos, c2);
		AddSphere(m_pRenderer, worldPos, c2);
		m_pThreeDWidget->renderWindow()->Render();
		renderWindow()->Render();
	}
	
}



void SliceWidget::DisplayToWorld(int displayPos[2], double worldPos[3])
{
	// First compute the equivalent of this display point on the focal plane
	double fp[4] = { 0.0 };
	double tmp1[4] = { 0.0 };
	double eventFPpos[4] = { 0.0 };

	m_pRenderer->GetActiveCamera()->GetFocalPoint(fp);
	fp[3] = 1.0;
	m_pRenderer->SetWorldPoint(fp);
	m_pRenderer->WorldToDisplay();
	m_pRenderer->GetDisplayPoint(tmp1);

	tmp1[0] = displayPos[0];
	tmp1[1] = displayPos[1];

	m_pRenderer->SetDisplayPoint(tmp1);
	m_pRenderer->DisplayToWorld();

	m_pRenderer->GetWorldPoint(eventFPpos);
	// Copy the result
	for (int i = 0; i < 3; i++)
		worldPos[i] = eventFPpos[i];
}

void SliceWidget::XYToLPS(double displayPos[3], double worldPos[3])
{
	double axialElements[16] = {
	 1, 0, 0, 0,
	 0, -1, 0, 0,
	 0, 0, -1, 0,
	 0, 0, 0, 1
	};
	vtkNew<vtkMatrix4x4> xyToSlice;
	vtkNew<vtkMatrix4x4> xyToLPSMat;
	int FieldOfView[3]{ 0 };
	double XYZOrigin[3]{ 0 };
	m_pImgData->GetDimensions(FieldOfView);
	double spacing[3]{ 1,1,1 };

	for (int i = 0; i < 3; i++)
	{
		//spacing[i] = this->FieldOfView[i] / this->Dimensions[i];
		xyToSlice->SetElement(i, i, spacing[i]);
		xyToSlice->SetElement(i, 3, -FieldOfView[i] / 2. + XYZOrigin[i]);
	}
	xyToSlice->SetElement(2, 3, 0.);

	vtkNew<vtkMatrix4x4> SliceToLPS;
	SliceToLPS->Identity();
	SliceToLPS->DeepCopy(axialElements);
	SliceToLPS->SetElement(0, 3, m_center[0]);
	SliceToLPS->SetElement(2, 3, m_center[0]);
	SliceToLPS->SetElement(3, 3, m_center[0]);
	SliceToLPS->Invert();
	vtkMatrix4x4::Multiply4x4(SliceToLPS, xyToSlice.GetPointer(), xyToLPSMat.GetPointer());
	double eventPositionXY[4] = {
	static_cast<double>(displayPos[0]),
	static_cast<double>(displayPos[1]),
	0.0,
	1.0 };
	double world[4]{ 0 };
	//memcpy(world, worldPos, sizeof(double) * 3);
	xyToLPSMat->MultiplyPoint(eventPositionXY, world);
	memcpy(worldPos, world, sizeof(double) * 3);

	std::cout << "worldPos:" << worldPos[0] << ","<< worldPos[1] << ","<< worldPos[2] << std::endl;
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


    //��״����
    double axialElements[16] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
   /* double axialElements[16] = {
     1, 0, 0, 0,
     0, 1, 0, 0,
     0, 0, 1, 0,
     0, 0, 0, 1
    };*/
    //ʸ״����
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
    //��״����
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

	m_center[0] = center[0] = origin[0] + spacing[0] * 0.5 * (extent[0] + extent[1]);
	m_center[1] = center[1] = origin[1] + spacing[1] * 0.5 * (extent[2] + extent[3]);
	m_center[2] = center[2] = origin[2] + spacing[2] * 0.5 * (extent[4] + extent[5]);

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
	std::cout << "dirrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << std::endl;
	dirMatrx4->Print(std::cout);

	vtkNew<vtkMatrix4x4> dirInvert;
	vtkMatrix4x4::Invert(dirMatrx4, dirInvert);
   //ResetResliceMatrix();
    switch (m_eSliceType)
    {
    case Empty:
        break;
    case VR3D1:
        break;
    case MPRA:
#if 1
		m_presliceMatrix->DeepCopy(axialElements);
		vtkMatrix4x4::Multiply4x4(dirInvert, m_presliceMatrix, m_presliceMatrix);
	/*	vtkMatrix4x4::Multiply4x4(m_presliceMatrix, dirMatrx4, m_presliceMatrix);*/
		m_presliceMatrix->SetElement(0, 3, center[0]);
		m_presliceMatrix->SetElement(1, 3, center[1]);
		m_presliceMatrix->SetElement(2, 3, center[2]);
#else
		m_presliceMatrix->DeepCopy(axialElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        //m_presliceTransform->Concatenate(dirMatrx4);
		m_presliceTransform->Translate(center[0], center[1], center[2]);
        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());

#endif
        break;
    case MPRC:
#if 1
		m_presliceMatrix->DeepCopy(coronalElements);
		vtkMatrix4x4::Multiply4x4(dirInvert, m_presliceMatrix, m_presliceMatrix);
	/*	vtkMatrix4x4::Multiply4x4(m_presliceMatrix, dirMatrx4, m_presliceMatrix);*/

		m_presliceMatrix->SetElement(0, 3, center[0]);
		m_presliceMatrix->SetElement(1, 3, center[1]);
		m_presliceMatrix->SetElement(2, 3, center[2]);
#else
        m_presliceMatrix->DeepCopy(coronalElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        //m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Translate(center[0], center[1], center[2]);
     
        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());
#endif
        break;
    case MPRS:
#if 1
		m_presliceMatrix->DeepCopy(sagittalElements);
		vtkMatrix4x4::Multiply4x4(dirInvert, m_presliceMatrix, m_presliceMatrix);
		/*vtkMatrix4x4::Multiply4x4(m_presliceMatrix, dirMatrx4, m_presliceMatrix);*/
		m_presliceMatrix->SetElement(0, 3, center[0]);
		m_presliceMatrix->SetElement(1, 3, center[1]);
		m_presliceMatrix->SetElement(2, 3, center[2]);
#else
        m_presliceMatrix->DeepCopy(sagittalElements);
        m_presliceTransform->SetMatrix(m_presliceMatrix);
        m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Translate(center[0], center[1], center[2]);

        m_presliceTransform->Update();
        m_presliceMatrix->DeepCopy(m_presliceTransform->GetMatrix());
#endif
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
		double color[3]{ 0,0,1 };
		double pos[3]{ 0 };
		//AddSphere(m_pRenderer, pos, color, m_presliceMatrix);
    }


    m_pRenderer->AddActor(m_pSliceImagePipeLine->m_pImageSliceActor);
    if (m_pThreeDWidget)
    {
        m_pThreeDWidget->GetRenderer()->AddActor(m_pSliceImagePipeLine->actorImageIn3D);
        //m_pThreeDWidget->GetRenderer()->ResetCamera();
        m_pThreeDWidget->renderWindow()->Render();
    }
    

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


    
	InitCamera(center, yd,dirInvert);

	//InitCamera2(center, yd);
	//InitMRHeadCamera(center, yd);

    //auto camera = m_pRenderer->GetActiveCamera();

    //camera->ParallelProjectionOn();
    //camera->SetViewAngle(30.0);

    //double distance = camera->GetDistance();
    //distance = 1;
    //double focalPoint[3];
    ////camera->GetParallelProjection();
    //camera->GetFocalPoint(focalPoint);
    //camera->SetParallelScale(0.5f * yd);
    //
    //double cameraPositon[3]{ center[0] + 0 * distance,
    //                      center[1] + 0 * distance,
    //                      center[2]  -1 * distance };
    //camera->SetPosition(cameraPositon);
    //camera->SetFocalPoint(center);
    //camera->SetViewUp(0, -1, 0);
    ////camera->SetParallelScale(0.5);

  
    m_presliceMatrix->Print(std:: cout);
   /* m_pRenderer->ComputeVisiblePropBounds();
    m_pRenderer->ResetCameraClippingRange();*/

   //m_pRenderer->ResetCamera();
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

void SliceWidget::InitCamera(double center[3],double scale, vtkMatrix4x4* dirMat)
{
	int extent[6]{ 0 };
	m_pImgData->GetExtent(extent);
	double sp[3];
	m_pImgData->GetSpacing(sp);
	double dx = (extent[1] - extent[0] + 1) * sp[0];
	double dy = (extent[3] - extent[2] + 1) * sp[1];
	double dz = (extent[5] - extent[4] + 1)*sp[2];
	auto camera = m_pRenderer->GetActiveCamera();

	camera->ParallelProjectionOn();
	camera->SetViewAngle(30.0);

	double distance = 100;

	double focalPoint[3]{0};
	double cameraPositon[3]{ 0 };
	//camera->GetParallelProjection();
	camera->GetFocalPoint(focalPoint);

	double viewup[4]{ 0,-1,0,0};
	double dirvec[4]{ 0,0,-1,0};
	/*vtkNew<vtkMatrix4x4> sliceToWorld;
	GetSliceToWorldOrientation(sliceToWorld);
	sliceToWorld->MultiplyPoint(viewup, viewup);
	sliceToWorld->MultiplyPoint(dirvec, dirvec);
	camera->SetParallelScale(0.5f * dz);
	cameraPositon[0] = center[0] + dirvec[0] * distance;
	cameraPositon[1] = center[1] + dirvec[1] * distance;
	cameraPositon[2] = center[2] + dirvec[2] * distance;
	camera->SetPosition(cameraPositon);
	camera->SetFocalPoint(center);
	camera->SetViewUp(viewup[0], viewup[1], viewup[2]);*/

	switch (m_eSliceType)
	{
	case MPRA:
		viewup[0] = 0;
		viewup[1] = -1;
		viewup[2] = 0;

		dirvec[0] = 0;
		dirvec[1] = 0;
		dirvec[2] = -1;

		dirMat->MultiplyPoint(viewup, viewup);
		dirMat->MultiplyPoint(dirvec, dirvec);

		camera->SetParallelScale(0.5f * dy);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);

		if (m_pThreeDWidget)
		{
			double c[3]{ 0,1,0 };
			double c2[3]{ 1,0,0 };
			/*AddSphere(m_pThreeDWidget->GetRenderer(), cameraPositon, c);
			AddSphere(m_pThreeDWidget->GetRenderer(), center, c2);

			AddSphere(m_pRenderer, cameraPositon, c);
			AddSphere(m_pRenderer, center, c2);

			m_pThreeDWidget->renderWindow()->Render();*/
		}


		break;
		
	case MPRC:
		viewup[0] = 0;
		viewup[1] = 0;
		viewup[2] = 1;

		dirvec[0] = 0;
		dirvec[1] = 1;
		dirvec[2] = 0;

		dirMat->MultiplyPoint(viewup, viewup);
		dirMat->MultiplyPoint(dirvec, dirvec);

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	case MPRS:
		viewup[0] = 0;
		viewup[1] = 0;
		viewup[2] = 1;

		dirvec[0] = 1;
		dirvec[1] = 0;
		dirvec[2] = 0;

		dirMat->MultiplyPoint(viewup, viewup);
		dirMat->MultiplyPoint(dirvec, dirvec);

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	}
}


void SliceWidget::InitCamera2(double center[3], double scale)
{
	int extent[6]{ 0 };
	m_pImgData->GetExtent(extent);
	double sp[3];
	m_pImgData->GetSpacing(sp);
	double dx = (extent[1] - extent[0] + 1) * sp[0];
	double dy = (extent[3] - extent[2] + 1) * sp[1];
	double dz = (extent[5] - extent[4] + 1)*sp[2];
	auto camera = m_pRenderer->GetActiveCamera();

	camera->ParallelProjectionOn();
	camera->SetViewAngle(30.0);

	double distance = 1;

	double focalPoint[3]{ 0 };
	double cameraPositon[3]{ 0 };
	//camera->GetParallelProjection();
	camera->GetFocalPoint(focalPoint);

	double viewup[4]{ 0,0,0,1 };
	double dirvec[4]{ 0,0,0,1 };

	switch (m_eSliceType)
	{
	case MPRA:
		viewup[0] = 0;
		viewup[1] = -1;
		viewup[2] = 0;

		dirvec[0] = 0;
		dirvec[1] = 0;
		dirvec[2] = -1;

		camera->SetParallelScale(0.5f * dy);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;

	case MPRC:
		viewup[0] = 0;
		viewup[1] = 0;
		viewup[2] = 1;

		dirvec[0] = 0;
		dirvec[1] = 1;
		dirvec[2] = 0;

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	case MPRS:
		viewup[0] = 0;
		viewup[1] = 0;
		viewup[2] = 1;

		dirvec[0] = 1;
		dirvec[1] = 0;
		dirvec[2] = 0;

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	}
}

void SliceWidget::GetSliceToWorldOrientation(vtkMatrix4x4 * sliceToWorld)
{
	if (sliceToWorld == nullptr)
		return;
	sliceToWorld->Identity();
	switch (m_eSliceType)
	{
	case MPRA:
		sliceToWorld->SetElement(0, 0, 1); sliceToWorld->SetElement(0, 1,0 ); sliceToWorld->SetElement(0, 2, 0);
		sliceToWorld->SetElement(1, 0, 0); sliceToWorld->SetElement(1, 1,1 ); sliceToWorld->SetElement(1, 2,0 );
		sliceToWorld->SetElement(2, 0,0 ); sliceToWorld->SetElement(2, 1, 0); sliceToWorld->SetElement(2, 2, 1);
		break;

	case MPRC:
		sliceToWorld->SetElement(0, 0, 1); sliceToWorld->SetElement(0, 1, 0); sliceToWorld->SetElement(0, 2, 0);
		sliceToWorld->SetElement(1, 0, 0); sliceToWorld->SetElement(1, 1, 0); sliceToWorld->SetElement(1, 2, -1);
		sliceToWorld->SetElement(2, 0, 0); sliceToWorld->SetElement(2, 1, 1); sliceToWorld->SetElement(2, 2, 0);
		break;
	case MPRS:
		sliceToWorld->SetElement(0, 0, 0); sliceToWorld->SetElement(0, 1, 0); sliceToWorld->SetElement(0, 2, 1);
		sliceToWorld->SetElement(1, 0, 1); sliceToWorld->SetElement(1, 1, 0); sliceToWorld->SetElement(1, 2, 0);
		sliceToWorld->SetElement(2, 0, 0); sliceToWorld->SetElement(2, 1, 1); sliceToWorld->SetElement(2, 2, 0);
		break;
	}
}

void SliceWidget::InitMRHeadCamera(double center[3], double scale)
{
	/*
	0 1 0 220.574
	0 0 -1 -17.1429
	-1 0 0 203.431
	0 0 0 1*/

	int extent[6]{ 0 };
	m_pImgData->GetExtent(extent);
	double sp[3];
	m_pImgData->GetSpacing(sp);
	double dx = (extent[1] - extent[0] + 1) * sp[0];
	double dy = (extent[3] - extent[2] + 1) * sp[1];
	double dz = (extent[5] - extent[4] + 1)*sp[2];
	auto camera = m_pRenderer->GetActiveCamera();

	camera->ParallelProjectionOn();
	camera->SetViewAngle(30.0);

	double distance = 100;

	double focalPoint[3]{ 0 };
	double cameraPositon[3]{ 0 };
	//camera->GetParallelProjection();
	camera->GetFocalPoint(focalPoint);

	double viewup[4]{ 0,0,0,1 };
	double dirvec[4]{ 0,0,0,1 };

	switch (m_eSliceType)
	{
	case MPRA:
		viewup[0] = -1;
		viewup[1] = 0;
		viewup[2] = 0;

		dirvec[0] = 0;
		dirvec[1] = 1;
		dirvec[2] = 0;

		

		camera->SetParallelScale(0.5f * dy);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;

		if (m_pThreeDWidget)
		{
			double c[3]{ 0,1,0 };
			double c2[3]{ 1,0,0 };
			AddSphere(m_pThreeDWidget->GetRenderer(), cameraPositon, c);
			AddSphere(m_pThreeDWidget->GetRenderer(), center, c2);

			AddSphere(m_pRenderer, cameraPositon, c);
			AddSphere(m_pRenderer, center, c2);

			m_pThreeDWidget->renderWindow()->Render();
		}


		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);




		
		break;

	case MPRC:
		viewup[0] = 0;
		viewup[1] = -1;
		viewup[2] = 0;

		dirvec[0] = 1;
		dirvec[1] = 0;
		dirvec[2] = 0;

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	case MPRS:
		viewup[0] = 0;
		viewup[1] = -1;
		viewup[2] = 0;

		dirvec[0] = 0;
		dirvec[1] = 0;
		dirvec[2] = -1;

		camera->SetParallelScale(0.5f * dz);
		cameraPositon[0] = center[0] + dirvec[0] * distance;
		cameraPositon[1] = center[1] + dirvec[1] * distance;
		cameraPositon[2] = center[2] + dirvec[2] * distance;
		camera->SetPosition(cameraPositon);
		camera->SetFocalPoint(center);
		camera->SetViewUp(viewup[0], viewup[1], viewup[2]);
		break;
	}
}

void SliceWidget::AddSphere(vtkRenderer* renderer,double pos[3],double c[3], vtkMatrix4x4* userM4)
{
	vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(0, 0, 0);
	sphereSource->SetRadius(3);
	sphereSource->SetThetaResolution(40);
	sphereSource->SetPhiResolution(40);
	sphereSource->Update();

	//create a mapper
	vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	sphereMapper->SetInputConnection(sphereSource->GetOutputPort());

	// create an actor
	vtkSmartPointer<vtkActor> sphereActor = vtkSmartPointer<vtkActor>::New();
	sphereActor->SetMapper(sphereMapper);
	sphereActor->GetProperty()->SetColor(c[0], c[1], c[2]);
	sphereActor->SetScale(1.0, 1, 1);
	if (userM4 == nullptr)
	{
		vtkNew<vtkMatrix4x4> mat;
		mat->Identity();
		mat->SetElement(0, 3, pos[0]);
		mat->SetElement(1, 3, pos[1]);
		mat->SetElement(2, 3, pos[2]);
		sphereActor->SetUserMatrix(mat);
	}
	else
	{
		sphereActor->SetUserMatrix(userM4);
	}
	

	renderer->AddActor(sphereActor);
}
