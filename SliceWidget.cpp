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

class SliceImagePipeLine
{
public:
    SliceImagePipeLine(vtkImageData* pImageData,vtkMatrix4x4* pReSliceMatrix,vtkTransform* pTransform,double origin[3])
    {
        m_pImageReslice = vtkSmartPointer<vtkImageReslice>::New();
        m_pImageReslice->SetOutputDimensionality(2);
        m_pImageReslice->SetInterpolationModeToLinear();
        m_pImageReslice->SetBackgroundLevel(-1024);
        m_pImageReslice->AutoCropOutputOn();
        m_pImageReslice->SetResliceAxes(pReSliceMatrix);
        m_pImageReslice->SetInputData(pImageData);
        //m_pImageReslice->SetOutputOrigin(origin);
        //m_pImageReslice->SetResliceTransform(pTransform);

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

        m_pImageSliceActor = vtkSmartPointer<vtkImageActor>::New();
        m_pImageSliceActor->GetMapper()->SetInputConnection(m_pImgMapToColors->GetOutputPort());
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

    InitReSliceMatrx();
}

void SliceWidget::SetThreeDWidget(ThreeDWidget* widget)
{
    m_pThreeDWidget = widget;

}

void SliceWidget::InitReSliceMatrx()
{


   /* static double sagittalElements[16] = {
        0, 0,-1, 0,
        1, 0, 0, 0,
        0,-1, 0, 0,
        0, 0, 0, 1 };

    static double coronalElements[16] = {
        1, 0, 0, 0,
        0, 0, 1, 0,
        0, -1,0, 0,
        0, 0, 0, 1 };

    static double axialElements[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1 };

    static double obliqueElements[16] = {
        1, 0, 0, 0,
        0, 0.866025, -0.5, 0,
        0, 0.5, 0.866025, 0,
        0, 0, 0, 1 };*/


    //Öá×´¾ØÕó
    double axialElements[16] = {
        1, 0, 0, 0,
        0, -1, 0, 0,
        0, 0, -1, 0,
        0, 0, 0, 1
    };
    //Ê¸×´¾ØÕó
   /* double sagittalElements[16] = {
        0, 0, -1, 0,
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 0, 1 };*/

    double sagittalElements[16] = {
    0, 0, -1, 0,
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

    std::cout << center[0] << " " << center[1] << " " << center[2] << " " << std::endl;
    double viewup[3]{ 0,1,0 };
    double viewforward[4]{ 0,0,1,1 };

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
   dirMatrx4->Invert();
    
    switch (m_eSliceType)
    {
    case Empty:
        break;
    case VR3D1:
        break;
    case MPRA:
        m_presliceMatrix->DeepCopy(axialElements);

        vtkMatrix4x4::Multiply4x4(dirMatrx4,m_presliceMatrix, m_presliceMatrix);
       // m_presliceTransform->SetMatrix(m_presliceMatrix);
        /*m_presliceTransform->Concatenate(m_pImgData->GetPhysicalToIndexMatrix());*/
        m_presliceMatrix->SetElement(0, 3, center[0]);
        m_presliceMatrix->SetElement(1, 3, center[1]);
        m_presliceMatrix->SetElement(2, 3, center[2]);

        //m_presliceTransform->SetMatrix(az);
        //m_presliceTransform->Translate(center);
        //m_presliceTransform->Concatenate(dirMatrx4);
        //m_presliceTransform->Translate(-center[0], -center[1], -center[2]);

      /*  m_presliceTransform->Concatenate(dirMatrx4);
        m_presliceTransform->Concatenate(m_presliceMatrix);
        m_presliceTransform->Translate(center);*/

        viewup[0] = m_presliceMatrix->Element[0][1];
        viewup[1] = m_presliceMatrix->Element[1][1];
        viewup[2] = m_presliceMatrix->Element[2][1];
       /*m_pRenderer->GetActiveCamera()->SetViewUp(viewup);*/
        //std::cout<<"xxxxxxxxxxxxxxxxxxxx" << viewup[0] << "," << viewup[1] << "," << viewup[2] << std::endl;
        break;
    case MPRC:
        m_presliceMatrix->DeepCopy(coronalElements);
      
        vtkMatrix4x4::Multiply4x4(dirMatrx4, m_presliceMatrix, m_presliceMatrix);
        //vtkMatrix4x4::Multiply4x4(ras2lps, m_presliceMatrix, m_presliceMatrix);
        m_presliceMatrix->SetElement(0, 3, center[0]);
        m_presliceMatrix->SetElement(1, 3, center[1]);
        m_presliceMatrix->SetElement(2, 3, center[2]);
        viewup[0] = m_presliceMatrix->Element[0][1];
        viewup[1] = m_presliceMatrix->Element[1][1];
        viewup[2] = m_presliceMatrix->Element[2][1];
        //m_pRenderer->GetActiveCamera()->SetViewUp(viewup);
       /* std::cout << "xxxxxxxxxxxxxxxxxxxx" << viewup[0] << "," << viewup[1] << "," << viewup[2] << std::endl;
        viewup[0] = -1;
        viewup[1] = 0;*/
        //m_pRenderer->GetActiveCamera()->SetViewUp(viewup);
        break;
    case MPRS:
        m_presliceMatrix->DeepCopy(sagittalElements);
        vtkMatrix4x4::Multiply4x4(dirMatrx4, m_presliceMatrix, m_presliceMatrix);
        //vtkMatrix4x4::Multiply4x4(ras2lps, m_presliceMatrix, m_presliceMatrix);
        m_presliceMatrix->SetElement(0, 3, center[0]);
        m_presliceMatrix->SetElement(1, 3, center[1]);
        m_presliceMatrix->SetElement(2, 3, center[2]);
        viewup[0] = m_presliceMatrix->Element[0][1];
        viewup[1] = m_presliceMatrix->Element[1][1];
        viewup[2] = m_presliceMatrix->Element[2][1];
  ///*      m_pRenderer->GetActiveCamera()->SetViewUp(viewup);*/
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
        m_pSliceImagePipeLine = std::make_shared<SliceImagePipeLine>(m_pImgData,m_presliceMatrix,m_presliceTransform,center);
    }


    m_pRenderer->AddActor(m_pSliceImagePipeLine->m_pImageSliceActor);
    if (m_pThreeDWidget)
    {
        m_pThreeDWidget->GetRenderer()->AddActor(m_pSliceImagePipeLine->m_pImageSliceActor);
        m_pThreeDWidget->GetRenderer()->ResetCamera();
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

    m_pRenderer->ResetCamera();
  
    renderWindow()->Render();

}

void SliceWidget::RenderCone()
{
    //
  // Next we create an instance of vtkNamedColors and we will use
  // this to select colors for the object and background.
  //
    vtkNew<vtkNamedColors> colors;

    //
    // Now we create an instance of vtkConeSource and set some of its
    // properties. The instance of vtkConeSource "cone" is part of a
    // visualization pipeline (it is a source process object); it produces data
    // (output type is vtkPolyData) which other filters may process.
    //
    vtkNew<vtkConeSource> cone;
    cone->SetHeight(3.0);
    cone->SetRadius(1.0);
    cone->SetResolution(10);

    //
    // In this example we terminate the pipeline with a mapper process object.
    // (Intermediate filters such as vtkShrinkPolyData could be inserted in
    // between the source and the mapper.)  We create an instance of
    // vtkPolyDataMapper to map the polygonal data into graphics primitives. We
    // connect the output of the cone source to the input of this mapper.
    //
    vtkNew<vtkPolyDataMapper> coneMapper;
    coneMapper->SetInputConnection(cone->GetOutputPort());




    //
    // Create an actor to represent the cone. The actor orchestrates rendering
    // of the mapper's graphics primitives. An actor also refers to properties
    // via a vtkProperty instance, and includes an internal transformation
    // matrix. We set this actor's mapper to be coneMapper which we created
    // above.
    //
    vtkNew<vtkActor> coneActor;
    coneActor->SetMapper(coneMapper);
    coneActor->GetProperty()->SetColor(colors->GetColor3d("MistyRose").GetData());



    // Create the Renderer and assign actors to it. A renderer is like a
// viewport. It is part or all of a window on the screen and it is
// responsible for drawing the actors it has.  We also set the background
// color here.
//
    vtkNew<vtkRenderer> ren1;
    ren1->AddActor(coneActor);
    ren1->SetBackground(colors->GetColor3d("MidnightBlue").GetData());


    renderWindow()->AddRenderer(ren1);
}
