// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterTaskGoTowardsRandomPoint.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "AdvancedHorrorAI/MonsterAI/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Navigation/PathFollowingComponent.h"

UMonsterTaskGoTowardsRandomPoint::UMonsterTaskGoTowardsRandomPoint()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UMonsterTaskGoTowardsRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	FVector Location = AIController->GetCharacter()->GetActorLocation();
	FNavLocation ResultLocation;
	if(NavSys->GetRandomReachablePointInRadius(Location, Radius, ResultLocation))
	{
		AIController->MoveToLocation(ResultLocation);
		return EBTNodeResult::InProgress;
	}

	UE_LOG(LogTemp, Warning, TEXT("Could not set a random point to roam to."))
	return EBTNodeResult::Failed;
}

void UMonsterTaskGoTowardsRandomPoint::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if(AIController->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
