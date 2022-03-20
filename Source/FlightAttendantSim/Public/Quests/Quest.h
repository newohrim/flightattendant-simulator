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
	UQuestNode* CreateNode() { return NewObject<UQuestNode>(this); }
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UQuestNode* Node) { CurrentNode = Node; }
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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
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
	UQuestNode* CurrentNode;
	// Is quest placed e.g. used for World Map generation?
	UPROPERTY(BlueprintReadOnly)
	bool IsPlaced = false;
	// Is quest familiar to Player?
	// It could be if player talked to one of the quest characters.
	UPROPERTY(BlueprintReadOnly)
	bool IsFamiliar = false;
	
	UFAGameInstance* GetFAGameInstance() const;
};
