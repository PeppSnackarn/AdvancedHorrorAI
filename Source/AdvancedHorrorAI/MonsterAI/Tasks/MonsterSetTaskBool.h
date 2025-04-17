// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterTaskBase.h"
#include "MonsterSetTaskBool.generated.h"

enum class EState : uint8;
/**
 * 
 */
UCLASS()
class ADVANCEDHORRORAI_API UMonsterSetStateTask : public UMonsterTaskBase
{
	GENERATED_BODY()
	UMonsterSetStateTask();
	
	UPROPERTY(EditAnywhere)
	EState ChangeStateTo;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
