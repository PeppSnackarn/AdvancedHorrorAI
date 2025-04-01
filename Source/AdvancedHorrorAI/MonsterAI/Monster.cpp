// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AIController.h"
#include "AdvancedHorrorAI/AdvancedHorrorAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"

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
		// since monster is having hard time tracking player, should find a way to make sure that the player is in sight of the monster or the director lets the monster cheat.
	}
	else
	{
		if(ShouldDecayAggression() && bDecayAgression)
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
		SetState(EState::Hunt); // Make hunting state not be affected by amount of aggression rather if above 80 aggression and sees player, insta hunt.
	}
}

void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
void AMonster::HandleSenses(AActor* Actor,FAIStimulus Stimulus) // If player has just ENTERED or EXITED the senses.
{
	TSubclassOf<UAISense> Sense = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), Stimulus);

	if(Sense->IsChildOf<UAISense_Sight>())
	{
		if(AAdvancedHorrorAICharacter* Player = Cast<AAdvancedHorrorAICharacter>(Actor))
		{
			bCanSeePlayer = !bCanSeePlayer;
			BlackboardComponent->SetValueAsBool("CanSeePlayer", bCanSeePlayer);
			SetLastSenseSensed(ELastSensedSense::Sight);
		}
	}
	if(Sense->IsChildOf<UAISense_Hearing>()) // make the aggression added scale with distance?
	{
		AddAggression(60); 
		BlackboardComponent->SetValueAsVector("LastHeardLocation", Actor->GetActorLocation());
		SetLastSenseSensed(ELastSensedSense::Hearing);
	}
	TimeAtLastSensedPlayer = GetWorld()->GetTime().GetRealTimeSeconds() + AggressionDecayCooldown;
}

bool AMonster::ShouldDecayAggression()
{
	return TimeAtLastSensedPlayer <= GetWorld()->GetTime().GetRealTimeSeconds();
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
void AMonster::SetLastSenseSensed(ELastSensedSense newSense)
{
	LastSensedSense = newSense;
	BlackboardComponent->SetValueAsEnum("LastSenseSensed", static_cast<uint8>(LastSensedSense));
	switch (newSense)
	{
	case ELastSensedSense::None:
		break;
	case ELastSensedSense::Sight:
		break;
	case ELastSensedSense::Hearing:
		break;
	}
}


