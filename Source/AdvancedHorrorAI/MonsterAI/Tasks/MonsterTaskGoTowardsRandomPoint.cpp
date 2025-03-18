// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTaskGoTowardsRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AdvancedHorrorAI/MonsterAI/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UMonsterTaskGoTowardsRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FVector Location = AIController->GetCharacter()->GetActorLocation();
	FNavLocation ResultLocation;
	NavSys->GetRandomReachablePointInRadius(Location, Radius, ResultLocation);
	AIController->MoveToLocation(ResultLocation);
	
	return EBTNodeResult::InProgress;
}
