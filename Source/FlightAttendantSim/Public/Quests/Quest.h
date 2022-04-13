// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldMap/LocationInfo.h"
#include "Quests/QuestNode.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

class UQuestNode;
class UFAGameInstance;
class ULocationInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FQuestFinished, UQuest*)

UENUM()
enum EQuestStatus
{
	Waiting		UMETA(DisplayName = "Waiting"),
	Taken		UMETA(DisplayName = "Taken"),
	Completed	UMETA(DisplayName = "Completed"),
	Failed		UMETA(DisplayName = "Failed")
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Abstract)
class FLIGHTATTENDANTSIM_API UQuest : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	FQuestFinished QuestFinished;
	
	UQuest();
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EQuestStatus> QuestStatus = EQuestStatus::Waiting;

	UFUNCTION(BlueprintCallable)
	UQuestNode* CreateNode()
	{  
		return CreateNodeIndexed(++LastNodeIndex);
	}
	UFUNCTION(BlueprintCallable)
	UQuestNode* CreateNodeIndexed(const int32 Index)
	{
		UQuestNode* Node = NewObject<UQuestNode>(this);
		Node->SetNodeIndex(Index);
		if (!RootNode)
		{
			RootNode = Node;
		}
		if (Index == CurrentNodeIndex)
		{
			SetCurrentNode(Node);
		}
		return Node;
	}
	UFUNCTION(BlueprintCallable)
	UQuestNode* GetCurrentNode() const { return CurrentNode; }
	// TODO: Move PreEvents call logic to ReconstructQuest()
	// Call only when all PreEvents for this node are set!
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UQuestNode* Node);
	void SetCurrentNodeIndex(const int32 NodeIndex) { CurrentNodeIndex = NodeIndex; }
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetCurrentGoals() const;
	UFUNCTION(BlueprintCallable)
	const TArray<ULocationInfo*>& GetLocationsToGenerate() const { return LocationsToGenerate; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetQuestName() const { return QuestName; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsPlaced () const { return IsPlaced; }
	UFUNCTION(BlueprintCallable)
	void SetIsPlaced(bool Value) { IsPlaced = true; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool GetIsFamiliar() const { return IsFamiliar; }
	UFUNCTION(BlueprintCallable)
	void SetIsFamiliar(bool Value) { IsFamiliar = Value; }

	void ReconstructQuest();
	// TODO: Make protected?
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	void TakeQuest();
	void ChangeNode(UQuestTransition* ExecutedTransition);
	void FinishQuest();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Quest", GetFName());
	}
	
protected:
	// Super helpful wrapper for GetAllActorsOfClass to call it from blueprint
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetTargetActors(const TSubclassOf<AActor> ClassToLookFor) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestInfo")
	FString QuestName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestInfo")
	FString ClientName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"), Category = "QuestInfo")
	FString QuestDescription;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = "QuestInfo")
	TArray<ULocationInfo*> LocationsToGenerate;
	UPROPERTY()
	UQuestNode* CurrentNode = nullptr;
	UPROPERTY()
	UQuestNode* RootNode = nullptr;
	// Is quest placed e.g. used for World Map generation?
	UPROPERTY(BlueprintReadOnly)
	bool IsPlaced = false;
	// Is quest familiar to Player?
	// It could be if player talked to one of the quest characters.
	UPROPERTY(BlueprintReadOnly)
	bool IsFamiliar = false;

	// Unique identifier of current node. Used for determine the actual current node. See CreateNode().
	int32 CurrentNodeIndex = -1;
	
	// Node index counter. Increments every time a new node gets created.
	int32 LastNodeIndex = -1;
	
	UFAGameInstance* GetFAGameInstance() const;

	UFUNCTION(BlueprintCallable)
	AGameModeBase* GetGameModeHelper() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static AGameModeBase* GetGameModeHelperStatic(const UObject* Outer);
};
