#ifndef COLORLOOKUPTABLE_H
#define COLORLOOKUPTABLE_H

#include <vtkLookupTable.h>
#include <QMap>

// 颜色查找表
class  ColorLookUpTable
{
public:

	//可选的伪彩方案
	static enum ColorType
	{
		Grey = 0,
		FullRainbow,
		//Iron,
		Rainbow,
		Ocean,
		Desert,
		//InvGrey,
		ReverseRainbow,
		FMRI,
		FMRIPA,
		Labels,
		//Obsolete,
		Random,
		Red,
		Green,
		Blue,
		Yellow,
		Cyan,
		Magenta,
		Warm1,
		Warm2,
		Warm3,
		Cool1,
		Cool2,
		Cool3,
		WarmShade1,
		WarmShade2,
		WarmShade3,
		CoolShade1,
		CoolShade2,
		CoolShade3,
		WarmTint1,
		WarmTint2,
		WarmTint3,
		CoolTint1,
		CoolTint2,
		CoolTint3,
		colorNumberMax
	};
	static std::string enum_str[];
	static vtkLookupTable * GetTable(int type);
	static std::string EnumToString(int type);


	static QMap<int, vtkLookupTable*> m_lookupTableMap;

};


#endif // COLORLOOKUPTABLE_H