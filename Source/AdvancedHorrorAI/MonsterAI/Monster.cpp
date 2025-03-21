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
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMonster::HandleSenses); // might not be the correct "tick" function
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
	}
	HandleAggressionStates();
	DecayAgression();
}

void AMonster::HandleAggressionStates()
{
	if(Aggression < 60)
	{
		SetState(EState::Idle);
	}
	else if(Aggression >= 60 && Aggression < 90) 
	{
		SetState(EState::Investigate);
	}
	else if(Aggression >= 90) 
	{
		SetState(EState::Hunt);
	}
	if(CurrentState == EState::Hunt)
	{
		// Add specific Hunt logic here???
	}
}

void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMonster::HandleSenses(AActor* Actor,FAIStimulus Stimulus)
{
	if (AAdvancedHorrorAICharacter* Player = Cast<AAdvancedHorrorAICharacter>(Actor))
	{
		bCanSeePlayer = !bCanSeePlayer;
		if(!bCanSeePlayer)
		{
			TimeAtLastSeenPlayer = GetWorld()->GetTime().GetRealTimeSeconds() + 5;
		}
		BlackboardComponent->SetValueAsVector("LastPlayerLocation", Player->GetActorLocation());
	}
}

void AMonster::DecayAgression()
{
	if(TimeAtLastSeenPlayer <= GetWorld()->GetTime().GetRealTimeSeconds())
	{
		AddAggression(-100);
	}
}

void AMonster::SetState(EState newState)
{
	CurrentState = newState;
	BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
	float InvestigateStartTime = 0;
	float HuntStartTime = 0;
	switch (newState)
	{
	case EState::Idle:
		break;
	case EState::Patrol:
		break;
	case EState::Investigate:
		InvestigateStartTime = GetWorld()->GetTimeSeconds(); // fire logic here
		break;
	case EState::Hunt:
		HuntStartTime = GetWorld()->GetTimeSeconds(); // fire logic here
		break;
	case EState::Leave:
		break;
	}
}


