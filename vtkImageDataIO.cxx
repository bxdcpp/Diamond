#include "vtkImageDataIO.h"

// VTK includes
#include <vtkImageData.h>
#include <vtkPolyDataAlgorithm.h>
#include <vtkNIFTIImageReader.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkMatrix4x4.h>
#include <vtkMatrix3x3.h>
#include <vtkExtractVOI.h>
#include <vtkTransform.h>
#include <vtkNIFTIImageHeader.h>

vtkStandardNewMacro(vtkImageDataIO);

//----------------------------------------------------------------------------
bool vtkImageDataIO::saveImageDataToNiiGz(std::string filePath, vtkImageData* imageData)
{
	if (imageData == nullptr || filePath.empty())
	{
		return false;
	}
	vtkNew<vtkImageData> outputImageData;
	//lps->ras
	LPSImageAndRASImageconverter(imageData, outputImageData);
	vtkNew<vtkMatrix4x4> m4;
	vtkMatrix3x3* imgM3 = outputImageData->GetDirectionMatrix();
	double origin[3]{ 0 };
	outputImageData->GetOrigin(origin);
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			m4->SetElement(i, j, imgM3->GetElement(i, j));
		}
		m4->SetElement(i, 3, origin[i]);
	}
	outputImageData->SetOrigin(0, 0, 0);
	vtkSmartPointer<vtkNIFTIImageWriter> writer = vtkSmartPointer<vtkNIFTIImageWriter>::New();
	writer->SetFileName(filePath.c_str());
	writer->SetInputData(outputImageData);
	//vtkNew<vtkNIFTIImageHeader> header;
	//writer->SetNIFTIHeader(header);
	//vtkNIFTIImageHeader* header = writer->GetNIFTIHeader();
	//auto type = header->GetDataType();
	//header->Print(std::cout);
	writer->SetQFormMatrix(m4);
	writer->SetSFormMatrix(m4);
	writer->Write();

	return true;
}

//----------------------------------------------------------------------------
bool vtkImageDataIO::readImageDataFromNiiGz(std::string filePath, vtkImageData* imageData)
{
	if (imageData == nullptr || filePath.empty())
	{
		return false;
	}
	vtkSmartPointer<vtkNIFTIImageReader> reader = vtkSmartPointer<vtkNIFTIImageReader>::New();
	reader->SetFileName(filePath.c_str());
	reader->Update();
	imageData->DeepCopy(reader->GetOutput());

	vtkSmartPointer<vtkMatrix4x4> formMatrix = reader->GetSFormMatrix();
	if (formMatrix == nullptr)
	{
		formMatrix = reader->GetQFormMatrix();
	}

	if (formMatrix)
	{
		vtkSmartPointer<vtkMatrix3x3> imageM3 = vtkSmartPointer<vtkMatrix3x3>::New();
		imageM3->Identity();
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				imageM3->SetElement(i, j, formMatrix->GetElement(i, j));
			}
		}

		imageData->SetDirectionMatrix(imageM3);
		//imageM3->Print(std::cout);
		imageData->SetOrigin(
			formMatrix->GetElement(0, 3),
			formMatrix->GetElement(1, 3),
			formMatrix->GetElement(2, 3));

		LPSImageAndRASImageconverter(imageData, imageData);
	}
	return true;
}

//----------------------------------------------------------------------------
void vtkImageDataIO::LPSImageAndRASImageconverter(vtkImageData* input, vtkImageData* output)
{
	if (input == nullptr || output == nullptr)
		return;
	vtkNew<vtkMatrix4x4> ijkTolpsMatrix;
	ijkTolpsMatrix->DeepCopy(input->GetIndexToPhysicalMatrix());

	ijkTolpsMatrix->Transpose();
	double origin[3]{ 0 };
	double mag[3]{ 0 };
	int i;
	for (i = 0; i < 3; i++)
	{
		// normalize vectors
		mag[i] = 0;
		for (int j = 0; j < 3; j++)
		{
			mag[i] += ijkTolpsMatrix->GetElement(i, j) * ijkTolpsMatrix->GetElement(i, j);
		}
		if (mag[i] == 0.0)
		{
			mag[i] = 1;
		}
		mag[i] = sqrt(mag[i]);
	}

	for (i = 0; i < 3; i++)
	{
		int j;
		for (j = 0; j < 3; j++)
		{
			ijkTolpsMatrix->SetElement(i, j, ijkTolpsMatrix->GetElement(i, j) / mag[i]);
		}
	}

	// convert from ijkToLps to ijkToRas
	vtkNew<vtkMatrix4x4> lpsToRasMatrix;
	lpsToRasMatrix->Identity();
	lpsToRasMatrix->SetElement(0, 0, -1);
	lpsToRasMatrix->SetElement(1, 1, -1);

	vtkNew<vtkMatrix4x4> ijkToRasMatrix;
	vtkMatrix4x4::Multiply4x4(ijkTolpsMatrix, lpsToRasMatrix, ijkToRasMatrix);

	vtkNew<vtkMatrix3x3> directionMatrix;
	for (i = 0; i < 3; i++)
	{
		origin[i] = ijkTolpsMatrix->GetElement(3, i);
		int j;
		for (j = 0; j < 3; j++)
		{
			directionMatrix->SetElement(j, i, ijkToRasMatrix->GetElement(i, j));
		}
	}

	if (input != output)
		output->DeepCopy(input);

	output->SetDirectionMatrix(
		directionMatrix
	);
	output->SetSpacing(mag);
	origin[0] *= -1;//L-R
	origin[1] *= -1;//p-S
	output->SetOrigin(origin);
	output->Modified();
}
