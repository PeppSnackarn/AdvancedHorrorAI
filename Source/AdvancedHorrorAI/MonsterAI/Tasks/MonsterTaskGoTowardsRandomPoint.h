// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MonsterTaskBase.h"
#include "MonsterTaskGoTowardsRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDHORRORAI_API UMonsterTaskGoTowardsRandomPoint : public UMonsterTaskBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category = "Random point search")
	float Radius = 500;
};
