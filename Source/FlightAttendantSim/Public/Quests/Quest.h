// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestNode.h"
#include "UObject/Object.h"
#include "Quest.generated.h"

class UQuestNode;

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
UCLASS(Blueprintable, BlueprintType)
class FLIGHTATTENDANTSIM_API UQuest : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EQuestStatus> QuestStatus = EQuestStatus::Waiting;

	UFUNCTION(BlueprintCallable)
	UQuestNode* CreateNode() { return NewObject<UQuestNode>(this); }
	UFUNCTION(BlueprintCallable)
	void SetCurrentNode(UQuestNode* Node) { CurrentNode = Node; }

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Init();
	UFUNCTION(BlueprintCallable)
	void TakeQuest();
	void ChangeNode(UQuestNode* NextNode);
	void FinishQuest();

protected:
	FString QuestName;
	FString QuestDescription;
	UPROPERTY()
	UQuestNode* CurrentNode;
};
