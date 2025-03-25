// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterTaskBase.h"
#include "MonsterLookAroundSelf.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDHORRORAI_API UMonsterLookAroundSelf : public UMonsterTaskBase
{
	GENERATED_BODY()
	UMonsterLookAroundSelf();
public:
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 90; // Degrees per second

private:
	float CurrentRotation = 0;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
