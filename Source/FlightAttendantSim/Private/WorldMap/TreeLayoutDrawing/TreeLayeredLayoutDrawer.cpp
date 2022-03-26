/*
*	Aesthetic Rules 
* 
*	1. Trees impose a distance on the nodes; no node should be closer to the root than any of its ancestors. 
*	2. Nodes at the same level of the tree should lie along a straight line, and the straight lines corresponding to the levels should be parallel. 
*	3. The relative order of nodes on any level should be the same as in the level order traversal of the tree. 
*	4. For a binary tree, a left child should be positioned to the left of its parent and a right child to the right. 
*	5. A parent should be centred over its children. 
*	6. A subtree of a given tree should be drawn the same way regardless of where it occurs in the tree. 
* 
* 
*	Note that at any level two subtrees can never be moved closer; they can only be moved apart. Also note that once a subtree is laid out, its shape is fixed. 
*	The RT algorithm satisfies all six aesthetic rules presented above. 
*	The above algorithm can be implemented in two traversals of the input binary tree which has an O(N) complexity, where N is the number of nodes of the tree to be drawn.
*	The first traversal (postorder) sets the child nodes positions relative to their parent.
*	For each vertex v, recursively computes the horizontal displacement of the left and right children of v with respect to v.
*	The second traversal (preorder) fixes absolute positions by accumulating the displacements on the path from each vertex to
*	the root for the x-coordinate, and by considering the depth of each vertex for the y-coordinate.
*	The crucial idea of the algorithm is to keep track of the contour of the sub-trees by special pointers, called threads,
*	such that whenever two sub-trees are joined, only the top part of the trees down to the lowest level of the smaller tree need to be taken into account.
*	The nodes are positioned on a fixed grid and are considered to have zero width.  In the postorder traversal part of the recursion is the merging of the
*	contours of the two subtrees. The left contour of a binary tree T with height h is the sequence of vertices v0, ..., vh
*	such that vi is the leftmost vertex of T with depth i. Similarly we can define the right subtree. The construction of the contour of the resulting tree
*	can be done in the following way: Suppose that we have two subtrees T1 and T2 and the rooted vertex r. T1 and T2 are the left and right subtrees of r respectively.
*	Every subtree has a unique left and right contour and the computation of the left and right contour of the resulting tree can be derived by the initial contours of the two subtrees
*/

#include "WorldMap/TreeLayoutDrawing/TreeLayeredLayoutDrawer.h"
#include "WorldMap/MapNode.h"

void FTreeLayeredLayoutDrawer::MakeGridLayout(UMapNode* TreeRoot)
{
	SetUpTree(TreeRoot);
	if (!TreeVertices.empty())
	{
		PostOrderOffset(TreeVertices[0]);
		PreOrderCoordCalc(TreeVertices[0]);
		TerminateData();
		TreeVertices.clear();
	}
}

void FTreeLayeredLayoutDrawer::SetUpTree(UMapNode* Node)
{
	FTreeVertex* Vertex = new FTreeVertex(Node);
	TreeVertices.push_back(Vertex);
	for (UMapNode* Child : Node->GetChildNodes())
	{
		Vertex->ChildVertices.push_back(TreeVertices.size());
		SetUpTree(Child);
	}
}

void FTreeLayeredLayoutDrawer::MergeContours(FTreeVertex* A, FTreeVertex* B)
{
	/*
	1. If both subtrees have the same height h, then the left contour of the resulting tree will be the left contour of T1 (left subtree) plus the rooted vertex r,
		and respectively the right contour will be the right contour of T2  (right subtree) plus the vertex r.    
	2. If the height of the left subtree is less than the height of the right subtree, then the contour of the resulting tree will be derived as follow: 
		The right contour of the resulting tree will be the right contour of the right subtree plus the rooted vertex r. 
		The left contour can be the result of the concatenation of two portions plus the rooted vertex r. 
		Let the height of the left contour of the left subtree be h, and its bottommost vertex be u. 
		Also, let the w vertex belong to the left contour of the right subtree and its depth is h+1. 
		Then, the left contour will consist of two portions: (plus the rooted vertex r) the left contour of the left subtree, 
		and the portion of the left contour of the right subtree from the vertex w until its bottommost vertex. 
	3. The case in which the left subtree has greater height than the right subtree is analogous to the previous one.
	*/

	if (A->LeftContour.size() < B->RightContour.size())
	{
		std::vector<int32> New_Contour = std::vector<int32>(
			B->LeftContour.begin(),
			B->LeftContour.end() - A->LeftContour.size());

		// Here is my solution for rooted trees where I calculate general offsets of contours and, before merging them together, 
		// I change the lower joint vertex respectively. This way the merged contour works properly with rooted trees.
		const int32 ContourLength = std::min(
			A->LeftContour.size(),
			B->LeftContour.size());
		int32 A_Joint_Offset = 0, B_Joint_Offset = 0;
		for (int32 i = 0; i < ContourLength; ++i)
		{
			A_Joint_Offset += A->LeftContour[A->LeftContour.size() - i - 1];
			B_Joint_Offset += B->LeftContour[B->LeftContour.size() - i - 1];
		}
		B_Joint_Offset += B->LeftContour[B->LeftContour.size() - ContourLength - 1];
		New_Contour[New_Contour.size() - 1] = B_Joint_Offset - A_Joint_Offset;

		// Merge
		New_Contour.insert(New_Contour.end(),
			A->LeftContour.begin(),
			A->LeftContour.end());
		A->LeftContour = New_Contour;
	}
	else if (A->LeftContour.size() > B->RightContour.size())
	{
		// The very same logic, but completely mirrored
		std::vector<int32> new_contour = std::vector<int32>(
			A->RightContour.begin(),
			A->RightContour.end() - B->RightContour.size());
		const int32 controur_length = std::min(
			A->RightContour.size(),
			B->RightContour.size());
		int32 a_joint_offset = 0, b_joint_offset = 0;
		for (int32 i = 0; i < controur_length; ++i) 
		{
			a_joint_offset += A->RightContour[A->RightContour.size() - i - 1];
			b_joint_offset += B->RightContour[B->RightContour.size() - i - 1];
		}
		a_joint_offset += A->RightContour[A->RightContour.size() - controur_length - 1];
		new_contour[new_contour.size() - 1] = a_joint_offset - b_joint_offset;
		new_contour.insert(new_contour.end(),
			B->RightContour.begin(),
			B->RightContour.end());
		B->RightContour = new_contour;
	}
}

void FTreeLayeredLayoutDrawer::MoveDrawings(FTreeVertex* A, FTreeVertex* B)
{
		// The smallest contour length
    	const int32 ContourLength = std::min(
    		A->RightContour.size(), 
    		B->LeftContour.size());
    	// Min possible distance between a and b (default is 2).
    	int32 MinPossibleD = 2;
    	int32 CurrentD = 2;
    	// Not sure if I need this condition.
    	if (ContourLength > 1) 
    	{
    		for (int32 i = 1; i < ContourLength; ++i) 
    		{
    			const int32 Change = B->LeftContour[B->LeftContour.size() - i - 1] - 
    				A->RightContour[A->RightContour.size() - i - 1];
    			CurrentD -= Change;
    			if (CurrentD > MinPossibleD)
    			{
    				MinPossibleD = CurrentD;
    			}
    		}
    	}
    	A->RelOffset = -MinPossibleD / 2;
    	B->RelOffset = MinPossibleD / 2;
    	A->LeftContour[A->LeftContour.size() - 1] = A->RelOffset;
    	A->RightContour[A->RightContour.size() - 1] = A->RelOffset;
    	B->LeftContour[B->LeftContour.size() - 1] = B->RelOffset;
    	B->RightContour[B->RightContour.size() - 1] = B->RelOffset;
}

void FTreeLayeredLayoutDrawer::PostOrderOffset(FTreeVertex* Vertex, const int32 Offset)
{
	if (Vertex->ChildVertices.size() > 0)
	{
		int32 N = Vertex->ChildVertices.size();
		for (int32 i = 0; i < N / 2; ++i)
		{
			const int32 CalcOffset = i - N / 2;
			PostOrderOffset(TreeVertices[Vertex->ChildVertices[i]], CalcOffset);
		}
		if ((N & 1) == 1)
		{
			PostOrderOffset(TreeVertices[Vertex->ChildVertices[N / 2]], 0);
		}
		for (int32 i = N / 2 + (N & 1); i < N; ++i)
		{
			const int32 CalcOffset = i - N / 2 + !(N & 1);
			PostOrderOffset(TreeVertices[Vertex->ChildVertices[i]], CalcOffset);
		}
		if (N > 1) 
		{
			MoveDrawings(TreeVertices[Vertex->ChildVertices[0]],
				TreeVertices[Vertex->ChildVertices[1]]);
			MergeContours(TreeVertices[Vertex->ChildVertices[0]],
				TreeVertices[Vertex->ChildVertices[1]]);
			// My addition for rooted trees
			if (N > 2) 
			{
				// I create a fictive vertex which plays a role of common contour of the whole depth level.
				// Think of this as a divide and conquer logic.
				FTreeVertex* FictiveV = new FTreeVertex(nullptr);
				for (int32 i = 2; i < N; ++i) 
				{
					// The leftmost common contour
					FictiveV->LeftContour = TreeVertices[Vertex->ChildVertices[0]]->LeftContour;
					// Current rightmost common contour which I compare to the [i] drawing
					FictiveV->RightContour = TreeVertices[Vertex->ChildVertices[i - 1]]->RightContour;
					MoveDrawings(FictiveV, TreeVertices[Vertex->ChildVertices[i]]);
					MergeContours(FictiveV, TreeVertices[Vertex->ChildVertices[i]]);
					// Respectively offset the [i] merged subtree, so it doesn't intersect its left [i - 1] neighbour
					TreeVertices[Vertex->ChildVertices[i]]->RelOffset =
						TreeVertices[Vertex->ChildVertices[i - 1]]->RelOffset + 
						TreeVertices[Vertex->ChildVertices[i]]->RelOffset - FictiveV->RelOffset;
				}
				delete FictiveV;
				// Relative offset of the whole depth level, so the parent vert is placed in the middle between its children
				int32 Avr = 0;
				for (int32 i = 0; i < N; ++i) 
				{
					Avr += TreeVertices[Vertex->ChildVertices[i]]->RelOffset;
				}
				Avr /= N; // float enjoyers be like: X_X
				for (int i = 0; i < N; ++i)
				{
					TreeVertices[Vertex->ChildVertices[i]]->RelOffset -= Avr;
				}
			}
		}
		// Set vertex's outer contour
		Vertex->LeftContour = TreeVertices[Vertex->ChildVertices[0]]->LeftContour;
		Vertex->RightContour = TreeVertices[Vertex->ChildVertices[N - 1]]->RightContour;
	}
	else 
	{
		// Leaf sets its precalculated relative offset here
		Vertex->RelOffset = Offset;
	}
	// Close the contour.
	// (i think i could send 0 as a contour offset, but just to be clear that leafs have correct contours).
	// (i manually update it in move_drawings method).
	Vertex->LeftContour.push_back(Vertex->RelOffset);
	Vertex->RightContour.push_back(Vertex->RelOffset);
}

void FTreeLayeredLayoutDrawer::PreOrderCoordCalc(FTreeVertex* Vertex, const int32 Depth)
{
	for (const int32 ChildIndex : Vertex->ChildVertices) 
	{
		FTreeVertex* Child = TreeVertices[ChildIndex];
		Child->NodePayload->SetHeightLevel(
			Vertex->NodePayload->GetHeightLevel() + Child->RelOffset);
		TreeVertices[ChildIndex]->NodePayload->SetDepth(Depth + 1);
		PreOrderCoordCalc(Child, Depth + 1);
	}
}

void FTreeLayeredLayoutDrawer::TerminateData()
{
	for (const FTreeVertex* Vertex : TreeVertices)
	{
		delete Vertex;
	}
}
