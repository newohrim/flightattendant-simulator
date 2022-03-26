#pragma once

#include "WorldMap/TreeLayoutDrawing/TreeLayoutDrawer.h"
#include <vector>

class UMapNode;

struct FTreeVertex
{
	UMapNode* NodePayload;

	// Relative horizontal offset from its parent
	int32 RelOffset = 0;

	std::vector<int32> ChildVertices;

	// The left contour of subtree T(this) based on horizontal offsets
	std::vector<int32> LeftContour;

	// The right contour of subtree T(this) based on horizontal offsets
	std::vector<int32> RightContour;

	FTreeVertex(UMapNode* Node) { NodePayload = Node; }
};

class FTreeLayeredLayoutDrawer : public ITreeLayoutDrawer
{
public:
	virtual void MakeGridLayout(UMapNode* TreeRoot) override;

private:
	std::vector<FTreeVertex*> TreeVertices;

	void SetUpTree(UMapNode* Node);

	// Merge left contours of a and right contours of b.
	// Result common left contour will be in a->left_contour, right contour will be in b->right_contour.
	void MergeContours(FTreeVertex* A, FTreeVertex* B);

	// Move two subtrees drawings horizontally, so the smallest horizontal distance between them is 2.
	// Result relative offsets will be in a->rel_offset, b->rel_offset.
	void MoveDrawings(FTreeVertex* A, FTreeVertex* B);

	// Postorder traversal of tree to calculate relative offsets of vertices relative to their parents.
	void PostOrderOffset(FTreeVertex* Vertex, const int32 Offset = 0);

	// Performs preorder traversal of tree to calculate actual 2D coordinates of vertices based on relative offsets from their parents.
	void PreOrderCoordCalc(FTreeVertex* Vertex, const int32 Depth = 0);
	
	void TerminateData();
};
