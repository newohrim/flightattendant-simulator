// Fill out your copyright notice in the Description page of Project Settings.


#include "TurbulenceData.h"

TArray<FVector2D> UTurbulenceData::GetFrame(FVector2D ClusterPos, const int32 NumOfEdges, const float Size, const float MaxOffset)
{
	TArray<FVector2D> Frame;

	Frame.Add({ ClusterPos.X + Size, ClusterPos.Y });
	for (int i = 0; i < NumOfEdges - 1; ++i)
	{
		const float Frac = (i + 1) / static_cast<float>(NumOfEdges);
		const float PointOnCircle = Frac * 2.0f * PI;
		FVector2D NextPoint = {
			FMath::Cos(PointOnCircle) * Size,
			FMath::Sin(PointOnCircle) * Size
		};
		FVector2D RandomOffset =
			FMath::FRand() * MaxOffset * NextPoint *
				(static_cast<float>(FMath::RandBool()) * 2.0f - 1.0f);
		Frame.Add(ClusterPos + NextPoint + RandomOffset);
	}

	return Frame;
}
