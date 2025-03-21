// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MonsterTaskBase.generated.h"

class AMonster;
/**
 * 
 */
UCLASS(Abstract)
class ADVANCEDHORRORAI_API UMonsterTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	AMonster* monsterRef; // Will be NULL if Super::ExecuteTask is not ran.
	AAIController* AIController; // Will be NULL if Super::ExecuteTask is not ran.
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
