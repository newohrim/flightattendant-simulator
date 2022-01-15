#pragma once

#include <vector>

typedef std::vector<std::pair<std::vector<bool>, std::vector<bool>>> FBmapHeight;

// So, the idea behind this class was that, if i find any intersections in this bitmap,
// I look for common ancestor of both intersected nodes and move the whole child upper or lower.
// There is still a question: how to know in which side to move the child graph?
// How to get a node from bitmap, if i don't have direct reference to this node from here?
// - the only possible solution is to recursively search for intersected node in the whole graph :(

class FBitmapHeight
{
public:
	FBitmapHeight() = default;
	FBitmapHeight(int NumOfColumns);

	bool GetBit(int Column, int HeightLevel) const;
	void SetBit(int Column, int HeightLevel, bool Value);

private:
	FBmapHeight Bitmap;
};
