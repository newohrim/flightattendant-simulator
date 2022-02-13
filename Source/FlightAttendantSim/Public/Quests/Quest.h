// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestNode.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

class UQuestNode;
class UFAGameInstance;

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
UCLASS(Blueprintable, BlueprintType, Abstract, EditInlineNew)
class FLIGHTATTENDANTSIM_API UQuest : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UQuest();
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EQuestStatus> QuestStatus = EQuestStatus::Waiting;

	UFUNCTION(BlueprintCallable)
	UQuestNode* CreateNode() { return NewObject<UQuestNode>(this); }
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UQuestNode* Node) { CurrentNode = Node; }
	UFUNCTION(BlueprintCallable)
	TArray<FString> GetCurrentGoals() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void TakeQuest();
	void ChangeNode(UQuestNode* NextNode);
	void FinishQuest();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("Quest", GetFName());
	}
	
protected:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<AActor*> GetTargetActors(const TSubclassOf<AActor> ClassToLookFor) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestInfo")
	FString QuestName;
	// TODO: Create character abstraction with all info on how and where to spawn him and move the name there.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "QuestInfo")
	FString ClientName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (MultiLine="true"), Category = "QuestInfo")
	FString QuestDescription;
	UPROPERTY()
	UQuestNode* CurrentNode;
	
	UFAGameInstance* GetFAGameInstance() const;
};
