

#ifndef __vtkImageDataIO_h
#define __vtkImageDataIO_h


// VTK includes
#include <vtkObject.h>


class vtkImageData;
//class vtkMatrix4x4;
//class vtkTransform;
//class vtkAbstractTransform;

class vtkImageDataIO : public vtkObject
{
public:
	static vtkImageDataIO* New();
	vtkTypeMacro(vtkImageDataIO, vtkObject);

	/// ngi.gz save
	static bool saveImageDataToNiiGz(std::string filePath, vtkImageData* imageData);
	/// ngi.gz read
	static bool readImageDataFromNiiGz(std::string filePath, vtkImageData* imageData);

	static bool saveImageROIFromNiiGz(std::string filePath, vtkImageData* inputData, vtkImageData* maskImageData);

	static bool saveImageDataToNrrd(std::string filePath, vtkImageData* imageData);
	static bool readImageDataFormNrrd(std::string filePath, vtkImageData* imageData);
	static void LPSImageAndRASImageconverter(vtkImageData* input, vtkImageData* output);
protected:
	vtkImageDataIO() = default;
	~vtkImageDataIO() = default;

private:
	vtkImageDataIO(const vtkImageDataIO&) = delete;
	void operator=(const vtkImageDataIO&) = delete;
};

#endif
