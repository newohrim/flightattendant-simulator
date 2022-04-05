// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/LocationInfo.h"

void ULocationInfo::Merge(ULocationInfo* Other)
{
	MergeUnique(Facilities, Other->Facilities);
	MergeUnique(ResidentsCharacters, Other->ResidentsCharacters);
	Other->CorrespondingNode = CorrespondingNode;
}

template <class T>
void ULocationInfo::MergeUnique(TArray<T>& A, const TArray<T>& B)
{
	TArray<T> Temp;
	for (int32 i = 0; i < B.Num(); ++i)
	{
		bool IsPresent = false;
		for (int32 j = 0; j < A.Num(); ++j)
		{
			if (B[i] == A[j])
			{
				IsPresent = true;
				break;
			}
		}
		if (!IsPresent)
		{
			Temp.Add(B[i]);
		}
	}
	A.Append(Temp);
}
