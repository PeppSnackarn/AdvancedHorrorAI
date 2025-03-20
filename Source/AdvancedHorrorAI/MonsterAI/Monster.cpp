// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AIController.h"
#include "AdvancedHorrorAI/AdvancedHorrorAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMonster::HandleSenses);
}
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	if (GetController())
		BlackboardComponent = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if (BlackboardComponent)
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

void AMonster::HandleSenses(AActor* Actor,FAIStimulus Stimulus)
{
	AAdvancedHorrorAICharacter* Player = Cast<AAdvancedHorrorAICharacter>(Actor);
	if (Player)
	{
		UE_LOG(LogTemp, Log, TEXT("Player spotted")) // does spot player but only when monster moves.
	}
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


