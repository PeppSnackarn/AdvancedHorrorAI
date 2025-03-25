// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterLookAroundSelf.h"

#include "AdvancedHorrorAI/MonsterAI/Monster.h"

UMonsterLookAroundSelf::UMonsterLookAroundSelf()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMonsterLookAroundSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UMonsterLookAroundSelf::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if(CurrentRotation >= 360)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

	float DeltaRotation = RotationSpeed * DeltaSeconds;
	CurrentRotation += DeltaRotation;
	FRotator NewRotation = monsterRef->GetActorRotation();
	NewRotation.Yaw += DeltaRotation;
	monsterRef->SetActorRotation(NewRotation);
}
