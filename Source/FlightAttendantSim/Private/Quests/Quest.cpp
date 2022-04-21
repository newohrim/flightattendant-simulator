// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/Quest.h"
#include "FAGameInstance.h"
#include "FAGameMode.h"
#include "QuestTransition.h"
#include "Kismet/GameplayStatics.h"
#include "Quests/QuestNode.h"

void UQuest::Init_Implementation()
{
	// PLACEHOLDER
}

UQuest::UQuest()
{
	UE_LOG(LogTemp, Display, TEXT("%s PARENT: %s"), *GetFullName(), *GetOuter()->GetFullName());
}

void UQuest::SetCurrentNode(UQuestNode* Node)
{
	if (!Node)
		return;
	
	CurrentNode = Node;
	//CurrentNode->ExecutePreEvents();
	
	CurrentNode->NodeCompleted.BindUObject(this, &UQuest::ChangeNode);
	CurrentNode->ExecutePreEvents();
	UE_LOG(LogTemp, Warning, TEXT("PRE EVENTS"));
}

TArray<FString> UQuest::GetCurrentGoals() const
{
	return CurrentNode->GetNodeGoals();
}

void UQuest::ReconstructQuest()
{
	LastNodeIndex = -1;
	Init();
}

void UQuest::TakeQuest()
{
	if (!CurrentNode || QuestStatus != EQuestStatus::Waiting)
		return;

	/* Happens in game mode now
	UWorld* World = GetWorld();
	if (!World)
		return;
	Cast<AFAGameMode>(World->GetAuthGameMode())->AddTakenQuest(this);
	*/

	CurrentNode->NodeCompleted.BindUObject(this, &UQuest::ChangeNode);
	QuestStatus = EQuestStatus::Taken;
	IsFamiliar = true;
}

void UQuest::ChangeNode(UQuestTransition* ExecutedTransition)
{
	if (QuestStatus != EQuestStatus::Taken)
		return;

	if (CurrentNode->ContainsTransition(ExecutedTransition))
	{
		ExecutedTransition->ExecutePostEvents();
		CurrentNode->NodeCompleted.Unbind();
		SetCurrentNode(ExecutedTransition->GetTargetNode());
		// Unable to place this condition inside SetCurrentNode,
		// because it is being called during quest construction.
		if (CurrentNode->IsLast())
		{
			FinishQuest();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s"), *GetName());
}

void UQuest::FinishQuest()
{
	QuestStatus = EQuestStatus::Completed;
	CurrentNode->NodeCompleted.Unbind();
	/* Happens in game mode now
	UWorld* World = GetWorld();
	if (!World)
		return;
	Cast<AFAGameMode>(World->GetAuthGameMode())->RemoveFinishedQuest(this);
	*/
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Quest completed."));
	QuestFinished.Broadcast(this);
}

TArray<AActor*> UQuest::GetTargetActors(const TSubclassOf<AActor> ClassToLookFor) const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToLookFor, FoundActors);
	return FoundActors;
}

UFAGameInstance* UQuest::GetFAGameInstance() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(World);
	if (!GameInstance)
	{
		return nullptr;
	}
	return Cast<UFAGameInstance>(GameInstance);
}

AGameModeBase* UQuest::GetGameModeHelper() const
{
	return GetGameModeHelperStatic(this);
}

AGameModeBase* UQuest::GetGameModeHelperStatic(const UObject* Outer)
{
	return UGameplayStatics::GetGameMode(Outer->GetWorld());
}
