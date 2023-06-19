#include "ColorLookUpTable.h"


QMap<int, vtkLookupTable*> ColorLookUpTable::m_lookupTableMap;

std::string ColorLookUpTable::enum_str[] = {
	"Grey",
		"FullRainbow",
		//Iron,
		"Rainbow",
		"Ocean",
		"Desert",
		//InvGrey,
		"ReverseRainbow",
		"FMRI",
		"FMRIPA",
		"Labels",
		//Obsolete,
		"Random",
		"Red",
		"Green",
		"Blue",
		"Yellow",
		"Cyan",
		"Magenta",
		"Warm1",
		"Warm2",
		"Warm3",
		"Cool1",
		"Cool2",
		"Cool3",
		"WarmShade1",
		"WarmShade2",
		"WarmShade3",
		"CoolShade1",
		"CoolShade2",
		"CoolShade3",
		"WarmTint1",
		"WarmTint2",
		"WarmTint3",
		"CoolTint1",
		"CoolTint2",
		"CoolTint3",
		"colorNumberMax"
};
 //设置指定的染色方案
vtkLookupTable * ColorLookUpTable::GetTable(int Type)
{
	//if (LookupTable != nullptr) {
	//	LookupTable->Delete();
	//	LookupTable = NULL;
	//}

	if (m_lookupTableMap.contains(Type))
	{
		return m_lookupTableMap[Type];
	}

	vtkLookupTable * LookupTable = vtkLookupTable::New();

	m_lookupTableMap.insert(Type, LookupTable);

	if (Type == FullRainbow)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 1);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1);
		LookupTable->Build();
		 //("A full rainbow of 256 colors, goes from red to red with all rainbow colors in between. Useful for colourful display of a label map");
	}
	else if (Type ==  Grey)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 0);
		LookupTable->SetSaturationRange(0, 0);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A grey scale ranging from black at 0 to white at 255. Useful for displaying MRI volumes.");
	}
	else if (Type ==  Red)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 0);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A red scale of 256 values. Useful for layering with Cyan");
	}
	else if (Type ==  Green)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.333, 0.333);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A green scale of 256 values, useful for layering with Magenta");
	}
	else if (Type ==  Blue)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.667, 0.667);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A blue scale of 256 values from black to pure blue, useful for layering with Yellow");
	}
	else if (Type ==  Yellow)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.167, 0.167);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A yellow scale of 256 values, from black to pure yellow, useful for layering with blue (it's complementary, layering yields gray)");
	}
	else if (Type ==  Cyan)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.5, 0.5);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A cyan ramp of 256 values, from black to cyan, complementary ramp to red, layering yields gray");
	}
	else if (Type ==  Magenta)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.833, 0.833);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A magenta scale of 256 colors from black to magenta, complementary ramp to green, layering yields gray ");
	}
	else if (Type ==  WarmShade1)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.167, 0.0);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to red, of 256 colors, ramp of warm colors with variation in value that's complementary to CoolShade1 ");
	}
	else if (Type ==  WarmShade2)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(.333, 0.167);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to yellow, through green, of 256 colors, ramp of warm colors with variation in value that's complementary to CoolShade2 ");
	}
	else if (Type ==  WarmShade3)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.5, 0.333);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to green, of 256 colours, ramp of warm colors with variation in value that's complementary to CoolShade3 ");
	}
	else if (Type ==  CoolShade1)
	{
		 //from vtkMRMLSliceLayerLogic.cxx
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.667, 0.5);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to cyan, 256 colours, ramp of cool colours with variation in value that is complementary to WarmShade1 ");
	}
	else if (Type ==  CoolShade2)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.833, 0.667);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to blue through purple, 256 colors, ramp of cool colours with variation in value that is complementary to WarmShade2 ");
	}
	else if (Type ==  CoolShade3)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(1.0, 0.833);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(0, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from black to magenta, 256 colours, ramp of cool colours with variation in value that is complementary to WarmShade3");
	}
	else if (Type ==  WarmTint1)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.167, 0.0);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to red, 256 colours, ramp of warm colours with variation in saturation that's complementary to CoolTint1");
	}
	else if (Type ==  WarmTint2)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(.333, 0.167);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to yellow, 256 colours, ramp of warm colours with variation in saturation that's complementary to CoolTint2");
	}
	else if (Type ==  WarmTint3)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.5, 0.333);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to green, 256 colours, ramp of warm colours with variation in saturation that's complementary to CoolTint3");
	}
	else if (Type ==  CoolTint1)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.667, 0.5);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to cyan, 256 colours, ramp of cool colours with variations in saturation that's complementary to WarmTint1");
	}
	else if (Type ==  CoolTint2)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.833, 0.667);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to blue, 256 colours, ramp of cool colours with variations in saturation that's complementary to WarmTint2");
	}
	else if (Type ==  CoolTint3)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(1.0, 0.833);
		LookupTable->SetSaturationRange(0, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from white to magenta, 256 colours, ramp of cool colours with variations in saturation that's complementary to WarmTint3");
	}
	else if (Type ==  Warm1)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.167, 0.0);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from yellow to red, of 256 colors, ramp of warm colours that's complementary to Cool1");
	}
	else if (Type ==  Warm2)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(.333, 0.167);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from green to yellow, 256 colours, ramp of warm colours that's complementary to Cool2");
	}
	else if (Type ==  Warm3)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.5, 0.333);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from cyan to green, 256 colours, ramp of warm colours that's complementary to Cool3");
	}
	else if (Type ==  Cool1)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.667, 0.5);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from blue to cyan, 256 colours, ramp of cool colours that's complementary to Warm1");
	}
	else if (Type ==  Cool2)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.833, 0.667);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from magenta to blue, 256 colours, ramp of cool colours that's complementary to Warm2");
	}
	else if (Type ==  Cool3)
	{
		LookupTable->SetRampToLinear();
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(1.0, 0.833);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetAlphaRange(1, 1); // not used
		LookupTable->Build();
		 //("A scale from red to magenta, ramp of cool colours that's complementary to Warm3");
	}
	//else if (Type ==  Iron)
	//{
	//	LookupTable->SetNumberOfTableValues(156);
	//	LookupTable->SetTableRange(0, 255);
	//	LookupTable->SetHueRange(0, 0.15);
	//	LookupTable->SetSaturationRange(1, 1);
	//	LookupTable->SetValueRange(1, 1);
	//	LookupTable->SetRampToLinear();
	//	LookupTable->Build();
	//	 //("A scale from red to  yellow, 157 colours, useful for ");
	//}
	else if (Type ==  Rainbow)
	{
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 0.8);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetRampToLinear();
		LookupTable->ForceBuild();
		 //("Goes from red to purple, passing through the colors of the rainbow in between. Useful for a colorful display of a label map");
	}
	else if (Type ==  Ocean)
	{
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.666667, 0.5);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetRampToLinear();
		LookupTable->Build();
		 //("A lighter blue scale of 256 values, useful for showing registration results.");
	}
	else if (Type ==  Desert)
	{
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 0.1);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetRampToLinear();
		LookupTable->ForceBuild();
		 //("Red to yellow/orange scale, 256 colours, useful for ");
	}
	//else if (Type ==  InvGrey)
	//{
	//	LookupTable->SetNumberOfTableValues(256);
	//	LookupTable->SetHueRange(0, 0);
	//	LookupTable->SetSaturationRange(0, 0);
	//	LookupTable->SetValueRange(1, 0);
	//	LookupTable->SetRampToLinear();
	//	LookupTable->Build();
	//	 //("A white to black scale, 256 colours, useful to highlight negative versions, or to flip intensities of signal values.");
	//}
	else if (Type ==  ReverseRainbow)
	{
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0.8, 1);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetRampToLinear();
		LookupTable->ForceBuild();
		 //("A colourful display option, 256 colours going from purple to red");
	}
	else if (Type ==  FMRI)
	{
		 //Use different numbers of table values for neg and pos
		 //to make sure -1 is represented by blue
		 //From green to blue
		vtkLookupTable *neg = vtkLookupTable::New();
		neg->SetNumberOfTableValues(23);
		neg->SetHueRange(0.5, 0.66667);
		neg->SetSaturationRange(1, 1);
		neg->SetValueRange(1, 1);
		neg->SetRampToLinear();
		neg->Build();

		 //From red to yellow
		vtkLookupTable *pos = vtkLookupTable::New();
		pos->SetNumberOfTableValues(20);
		pos->SetHueRange(0, 0.16667);
		pos->SetSaturationRange(1, 1);
		pos->SetValueRange(1, 1);
		pos->SetRampToLinear();
		pos->Build();

		LookupTable->SetNumberOfTableValues(43);
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetRampToLinear();
		LookupTable->Build();

		for (int i = 0; i < 23; i++)
		{
			LookupTable->SetTableValue(i, neg->GetTableValue(i));
		}
		for (int i = 0; i < 20; i++)
		{
			LookupTable->SetTableValue(i + 23, pos->GetTableValue(i));
		}

		pos->Delete();
		neg->Delete();
		 //("A combination of Ocean (0-22) and Desert (23-42), useful for displaying functional MRI volumes (highlights activation)");
	}
	else if (Type ==  FMRIPA)
	{
		int size = 20;
		LookupTable->SetNumberOfTableValues(size);
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetHueRange(0, 0.16667);
		LookupTable->SetSaturationRange(1, 1);
		LookupTable->SetValueRange(1, 1);
		LookupTable->SetRampToLinear();
		LookupTable->Build();
		 //("A small fMRI positive activation scale going from red to yellow from 0-19, useful for displaying functional MRI volumes when don't need the blue of the fMRI scale.");
	}
	else if (Type ==  Labels)
	{
		// //from Slicer2's Colors.xml
		LookupTable->SetNumberOfTableValues(257);
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetTableValue(0, 0, 0, 0, 0.0);
		LookupTable->SetTableValue(1, 0.2, 0.5, 0.8, 1.0);
		LookupTable->SetTableValue(2, 1.0, 0.8, 0.7, 1.0);
		LookupTable->SetTableValue(3, 1.0, 1.0, 1.0, 1.0);
		LookupTable->SetTableValue(4, 0.4, 0.7, 1.0, 1.0);
        LookupTable->SetTableValue(5, 0.9, 0.5, 0.5, 1.0);
		LookupTable->SetTableValue(6, 0.5, 0.9, 0.5, 1.0);
		LookupTable->SetTableValue(7, 0.5, 0.9, 0.9, 1.0);
		LookupTable->SetTableValue(8, 0.9, 0.9, 0.5, 1.0);
		LookupTable->SetTableValue(9, 0.9, 0.7, 0.9, 1.0);
		LookupTable->SetTableValue(10, 0.9, 0.9, 0.5, 1.0);
		for (int offset = 0; offset <= 240; offset += 10)
		{
			LookupTable->SetTableValue(offset + 11, 0.2, 0.5, 0.8, 1.0);
			LookupTable->SetTableValue(offset + 12, 0.2, 0.8, 0.5, 1.0);
			LookupTable->SetTableValue(offset + 13, 0.8, 0.5, 0.2, 1.0);
			LookupTable->SetTableValue(offset + 14, 0.8, 0.2, 0.5, 1.0);
			LookupTable->SetTableValue(offset + 15, 0.5, 0.2, 0.8, 1.0);
			LookupTable->SetTableValue(offset + 16, 0.5, 0.8, 0.2, 1.0);
			if (offset <= 230)
			{
				LookupTable->SetTableValue(offset + 17, 0.2, 0.2, 0.8, 1.0);
				LookupTable->SetTableValue(offset + 18, 0.8, 0.8, 0.2, 1.0);
				LookupTable->SetTableValue(offset + 19, 0.2, 0.8, 0.8, 1.0);
				LookupTable->SetTableValue(offset + 20, 0.5, 0.5, 0.5, 1.0);
			}
		}

		 //("A legacy colour table that contains some anatomical mapping");
	}
	else if (Type ==  Random)
	{
		int size = 255;

		LookupTable->SetNumberOfTableValues(size + 1);
		LookupTable->SetTableRange(0, 255);
		LookupTable->SetTableValue(0, 0, 0, 0, 0);
		for (int i = 1; i <= size; i++)
		{
			 //table values have to be 0-1
			double r = (rand() % 255) / 255.0;
			double g = (rand() % 255) / 255.0;
			double b = (rand() % 255) / 255.0;

			LookupTable->SetTableValue(i, r, g, b, 1.0);
		}
		 //("A random selection of 256 rgb colours, useful to distinguish between a small number of labeled regions (especially outside of the brain)");
	}
	
	return LookupTable;
}

std::string ColorLookUpTable::EnumToString(int type)
{
	return enum_str[type];
}
