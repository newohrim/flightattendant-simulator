// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GeomTools.h"
#include "TurbulenceData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class FLIGHTATTENDANTSIM_API UTurbulenceData : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static TArray<FVector2D> GetFrame(
		FVector2D ClusterPos,
		const int32 NumOfEdges,
		const float Size,
		const float MaxOffset);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsInsideTriangle(
		const FVector& P,
		const FVector& A,
		const FVector& B,
		const FVector& C)
	{ return FGeomTools::PointInTriangle(A, B, C, P); }
};
