// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGame/SaveGameComponent.h"

#include "FAGameMode.h"
#include "WorldMap/LocationInfo.h"
#include "Components/GameEconomyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/FASaveGame.h"
#include "Quests/Quest.h"

DEFINE_LOG_CATEGORY_STATIC(LogSaveGameComponent, All, All);

// Sets default values for this component's properties
USaveGameComponent::USaveGameComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool USaveGameComponent::SaveGame()
{
	UE_LOG(LogSaveGameComponent, Display, TEXT("Start saving..."));
	bool Success = false;
	UFASaveGame* SaveGame =
		Cast<UFASaveGame>(UGameplayStatics::CreateSaveGameObject(UFASaveGame::StaticClass()));
	if (SaveGame)
	{
		PopulateSaveFile(SaveGame);
		SaveGame->SaveSlotName = MainSaveGameSlotName;
		SaveGame->UserIndex = MainSaveGameSlotIndex;
		Success = UGameplayStatics::SaveGameToSlot(
			SaveGame, SaveGame->SaveSlotName, SaveGame->UserIndex);
	}
	if (Success)
	{
		UE_LOG(LogSaveGameComponent, Display,
			TEXT("Successfully created save file %s.sav"), *SaveGame->SaveSlotName);
	}
	else
	{
		UE_LOG(LogSaveGameComponent, Error, TEXT("Save game failed."));
	}
	return Success;
}

bool USaveGameComponent::LoadGame()
{
	bool Success = false;
	UFASaveGame* SaveGame =
		Cast<UFASaveGame>(UGameplayStatics::LoadGameFromSlot(MainSaveGameSlotName, MainSaveGameSlotIndex));
	if (SaveGame)
	{
		SaveGame->PostLoadInitialization();
		GatherLoadedSaveFile(SaveGame);
		Success = true;
	}
	if (Success)
	{
		UE_LOG(LogSaveGameComponent, Display,
			TEXT("Successfully loaded save file %s.sav"), *SaveGame->SaveSlotName);
	}
	else
	{
		UE_LOG(LogSaveGameComponent, Error, TEXT("Load game failed."));
	}
	return Success;
}

void USaveGameComponent::PopulateSaveFile(UFASaveGame* SaveGame)
{
	UWorld* World = GetWorld();
	if (World)
	{
		const AFAGameMode* GameMode = Cast<AFAGameMode>(World->GetAuthGameMode());
		const UGameEconomyComponent* EconomyComponent = GameMode->GetEconomyComponent();
		SaveGame->PlayerMoney = EconomyComponent->GetPlayerMoney();
		SaveGame->SaveWorldMap(GameMode->GetWorldMap());
		PopulateQuests(SaveGame->TakenQuests, GameMode->GetTakenQuests());
		PopulateQuests(SaveGame->PlacedQuests, GameMode->GetPlacedQuests());
		PopulateQuests(SaveGame->FinishedQuests, GameMode->GetFinishedQuests());
	}
}

void USaveGameComponent::GatherLoadedSaveFile(UFASaveGame* SaveGame)
{
	UWorld* World = GetWorld();
	if (World)
	{
		AFAGameMode* GameMode = Cast<AFAGameMode>(World->GetAuthGameMode());
		UGameEconomyComponent* EconomyComponent = GameMode->GetEconomyComponent();
		EconomyComponent->SetPlayerMoney(SaveGame->PlayerMoney);
		UMapGraph* WorldMap = GameMode->GetWorldMap();
		WorldMap->GenerateMap(
			ReconstructWorldMap(SaveGame->GetRootNode(), SaveGame));
		GameMode->InitQuestsFromSaveFile(
			GatherQuests(SaveGame->TakenQuests, EQuestStatus::Taken, GameMode),
			GatherQuests(SaveGame->PlacedQuests, EQuestStatus::Waiting, GameMode),
			GatherQuests(SaveGame->FinishedQuests, EQuestStatus::Completed, GameMode));
	}
}

void USaveGameComponent::PopulateQuests(TArray<FQuestData>& ToList, const TArray<UQuest*>& FromList)
{
	for (const UQuest* TakenQuest : FromList)
	{
		FQuestData QuestData = {
			TakenQuest->GetClass(),
			TakenQuest->GetCurrentNode()->GetNodeIndex()
		};
		ToList.Add(QuestData);
	}
}

TArray<UQuest*> USaveGameComponent::GatherQuests(
	const TArray<FQuestData>& FromList,
	const EQuestStatus QuestStatus,
	UObject* Outer)
{
	TArray<UQuest*> ResultQuests;
	for (const FQuestData& QuestData : FromList)
	{
		UQuest* Quest = NewObject<UQuest>(Outer, QuestData.QuestClass.Get());
		Quest->SetCurrentNodeIndex(QuestData.CurrentNodeIndex);
		Quest->QuestStatus = QuestStatus;
		// I know the actual pointer to current node after this call
		Quest->ReconstructQuest();
		ResultQuests.Add(Quest);
	}
	return ResultQuests;
}

UMapNode* USaveGameComponent::ReconstructWorldMap(
	const FMapNodeData& NodeData,
	const UFASaveGame* SaveGame,
	UObject* Outer)
{
	if (!Outer)
		Outer = Cast<AFAGameMode>(GetWorld()->GetAuthGameMode())->GetWorldMap();
	UMapNodeReconstructed* Node = NewObject<UMapNodeReconstructed>(Outer);
	Node->SetLocationInfo(NodeData.LocationInfo.Get());
	Node->GetLocationInfo()->CorrespondingNode = Node;
	if (NodeData.IsCurrent)
	{
		Cast<AFAGameMode>(GetWorld()->GetAuthGameMode())
			->GetWorldMap()->SetCurrentNode(Node);
	}
	if (UMapNode* Parent = Cast<UMapNode>(Outer))
	{
		Node->SetParentNode(Parent);
	}
	for (const int32 ChildIndex : NodeData.Children) 
	{
		Node->ChildNodes.Add(
			ReconstructWorldMap(SaveGame->WorldMap[ChildIndex], SaveGame, Node));
	}
	return Node;
}
