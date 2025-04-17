// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterSetTaskBool.h"

#include "AdvancedHorrorAI/MonsterAI/Monster.h"

UMonsterSetStateTask::UMonsterSetStateTask()
{
	NodeName = "Set state task";
}

EBTNodeResult::Type UMonsterSetStateTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	monsterRef->SetState(ChangeStateTo);
	return EBTNodeResult::Succeeded;
}
