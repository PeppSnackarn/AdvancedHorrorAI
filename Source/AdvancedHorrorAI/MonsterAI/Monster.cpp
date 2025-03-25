// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AIController.h"
#include "AdvancedHorrorAI/AdvancedHorrorAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

// Sets default values
AMonster::AMonster()
{
	PrimaryActorTick.bCanEverTick = true;
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMonster::HandleSenses); //Upon an actor having been sensed or stopped being sensed.
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
	if(bCanSeePlayer)
	{
		AddAggression(AggressionAddedPerSecond * DeltaTime);
		BlackboardComponent->SetValueAsVector("LastPlayerLocation", GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()); // Ugly solution to always have last player location available
		TimeAtLastSeenPlayer = GetWorld()->GetTime().GetRealTimeSeconds() + AggressionDecayCooldown;
	}
	else
	{
		if(ShouldDecayAggression())
		{
			AddAggression(-AggressionDecayPerSecond * DeltaTime);
		}
	}
	HandleAggressionStates();
}

void AMonster::HandleAggressionStates()
{
	if(Aggression < 60)
	{
		SetState(EState::Idle);
	}
	else if(Aggression >= 60 && Aggression < 80) 
	{
		SetState(EState::Investigate);
	}
	else if(Aggression >= 80) 
	{
		SetState(EState::Hunt);
	}
}

void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMonster::HandleSenses(AActor* Actor,FAIStimulus Stimulus) // If player has just ENTERED or EXITED the senses.
{
	if (AAdvancedHorrorAICharacter* Player = Cast<AAdvancedHorrorAICharacter>(Actor)) // only works with sight atm
	{
		bCanSeePlayer = !bCanSeePlayer;
		BlackboardComponent->SetValueAsBool("CanSeePlayer", bCanSeePlayer);
	}
}

bool AMonster::ShouldDecayAggression()
{
	return TimeAtLastSeenPlayer <= GetWorld()->GetTime().GetRealTimeSeconds();
}

void AMonster::SetState(EState newState)
{
	CurrentState = newState;
	BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
	switch (newState)
	{
	case EState::Idle:
		GetCharacterMovement()->MaxWalkSpeed = 300; // temp
		break;
	case EState::Patrol:
		break;
	case EState::Investigate:
		GetCharacterMovement()->MaxWalkSpeed = 400; // temp
		break;
	case EState::Hunt:
		GetCharacterMovement()->MaxWalkSpeed = 600; // temp
		break;
	case EState::Leave:
		break;
	}
}


