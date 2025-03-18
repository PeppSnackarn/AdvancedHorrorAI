// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MonsterTaskBase.generated.h"

class AMonster;
/**
 * 
 */
UCLASS()
class ADVANCEDHORRORAI_API UMonsterTaskBase : public UBTTaskNode
{
	GENERATED_BODY()
public:
	// Will be NULL if Super::ExecuteTask is not ran.
	AMonster* monsterRef;
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
