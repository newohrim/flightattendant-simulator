// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/FADocumentsInfo.h"
#include "DocsInfoStruct.h"

// Sets default values for this component's properties
UFADocumentsInfo::UFADocumentsInfo()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UFADocumentsInfo::BeginPlay()
{
	Super::BeginPlay();

	//GetWorld()->GetAuthGameMode();
}

void UFADocumentsInfo::SetDocumentsInfo(UDocsInfoStruct* Info) 
{
	DocsInfo = Info;
}

UDocsInfoStruct* UFADocumentsInfo::GetDocsInfo() 
{
	return DocsInfo;
}