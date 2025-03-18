// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTaskBase.h"
#include "AdvancedHorrorAI/MonsterAI/Monster.h"
#include "AIController.h"

EBTNodeResult::Type UMonsterTaskBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	monsterRef = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());
	
	return EBTNodeResult::Succeeded;
}
