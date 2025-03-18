// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTestTask.h"

#include "AdvancedHorrorAI/MonsterAI/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMonsterTestTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	monsterRef->SetState(EState::Patrol);
	
	return EBTNodeResult::Succeeded;
}
