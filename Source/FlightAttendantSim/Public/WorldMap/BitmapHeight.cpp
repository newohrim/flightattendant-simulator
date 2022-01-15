#include "BitmapHeight.h"

FBitmapHeight::FBitmapHeight(int NumOfColumns)
{
	Bitmap = FBmapHeight(NumOfColumns);
}

bool FBitmapHeight::GetBit(int Column, int HeightLevel) const
{
	if (Column >= Bitmap.size())
	{
		return false;
	}

	if (HeightLevel >= 0)
	{
		if (HeightLevel >= Bitmap[Column].first.size())
		{
			return false;
		}
		
		// Upper part
		return Bitmap[Column].first[HeightLevel];
	}
	else
	{
		HeightLevel = std::abs(HeightLevel);
		if (HeightLevel >= Bitmap[Column].first.size())
		{
			return false;
		}
		
		// Lower part
		return Bitmap[Column].second[HeightLevel];
	}
}

void FBitmapHeight::SetBit(int Column, int HeightLevel, bool Value)
{
	if (Column >= Bitmap.size())
	{
		Bitmap.resize(Column + 1);
	}

	if (HeightLevel >= 0)
	{
		if (HeightLevel >= Bitmap[Column].first.size())
		{
			Bitmap[Column].first.resize(HeightLevel + 1, false);
		}
		
		// Upper part
		Bitmap[Column].first[HeightLevel] = Value;
	}
	else
	{
		HeightLevel = std::abs(HeightLevel);
		if (HeightLevel >= Bitmap[Column].first.size())
		{
			Bitmap[Column].first.resize(HeightLevel + 1, false);
		}
		
		// Lower part
		Bitmap[Column].second[HeightLevel] = Value;
	}
}
