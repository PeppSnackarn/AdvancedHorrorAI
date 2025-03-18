// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "MonsterTaskBase.h"
#include "MonsterTestTask.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEDHORRORAI_API UMonsterTestTask : public UMonsterTaskBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
