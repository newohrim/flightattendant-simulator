// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
//#include "FASaveGameArchive.generated.h"

/**
 * 
 */
struct FFASaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FFASaveGameArchive(FArchive& InInnerArchive) 
	            : FObjectAndNameAsStringProxyArchive(InInnerArchive,true)
	{
		ArIsSaveGame = true;
		ArNoDelta = true; 
	}
};
