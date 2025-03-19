// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
}
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	BlackboardComponent = Cast<AAIController>(GetController())->GetBlackboardComponent();
	BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
}
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMonster::SetState(EState newState)
{
	CurrentState = newState;
	BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
	switch (newState)
	{
	case EState::Idle:
		break;
	case EState::Patrol:
		break;
	case EState::Investigate:
		break;
	case EState::Hunt:
		break;
	case EState::Leave:
		break;
	}
}


