// Fill out your copyright notice in the Description page of Project Settings.


#include "SpacePlane/Seat.h"


// Sets default values
ASeat::ASeat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SeatMesh = CreateDefaultSubobject<UStaticMeshComponent>("SeatMesh");
	SeatMesh->SetupAttachment(GetRootComponent());
	SnapLocation = CreateDefaultSubobject<USceneComponent>("SnapLocation");
	SnapLocation->SetupAttachment(SeatMesh);
	MoveToLocation = CreateDefaultSubobject<USceneComponent>("MoveLocation");
	MoveToLocation->SetupAttachment(SeatMesh);
}

// Called when the game starts or when spawned
void ASeat::BeginPlay()
{
	Super::BeginPlay();
	
}

