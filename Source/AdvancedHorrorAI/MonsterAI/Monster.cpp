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
	PerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &AMonster::HandleSenses);
	GetCharacterMovement()->MaxWalkSpeed = 300;
}
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	if (GetController())
		BlackboardComponent = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if (BlackboardComponent)
		BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));

	MonsterDefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
}
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	/*UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(UAISense::GetSenseID(UAISense_Sight::StaticClass())));
	if(SightConfig)
	{
		SightConfig->SightRadius = 50000; // IT WORKS!
		SightConfig->PeripheralVisionAngleDegrees = 100;
	}
	*/
	if(bCanSeePlayer)
	{
		AddAggression(AggressionAddedPerSecond * DeltaTime);
		BlackboardComponent->SetValueAsVector("LastPlayerLocation", GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()); // Ugly solution to always have last player location available
		// since monster is having hard time tracking player, should find a way to make sure that the player is in sight of the monster or the director lets the monster cheat.
		if(Aggression >= 60 && Aggression < 80)
		{
			SetState(EState::Investigate);
		}
		else if(Aggression >= 80) 
		{
			SetState(EState::Hunt); // Make hunting state not be affected by amount of aggression rather if above 80 aggression and sees player, insta hunt.
		}
	}
	else
	{
		if(ShouldDecayAggression() && bDecayAgression)
		{
			AddAggression(-AggressionDecayPerSecond * DeltaTime);
		}
	}
	if(Aggression < 60)
	{
		SetState(EState::Idle);
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
			SetLastSenseSensed(ELastSensedSense::Sight);
			bCanSeePlayer = !bCanSeePlayer;
			BlackboardComponent->SetValueAsBool("CanSeePlayer", bCanSeePlayer);
		}
	}
	if(Sense->IsChildOf<UAISense_Hearing>()) // make the aggression added scale with distance?
	{
		SetLastSenseSensed(ELastSensedSense::Hearing);
		AddAggression(60); 
		BlackboardComponent->SetValueAsVector("LastHeardLocation", Actor->GetActorLocation());
		if(Aggression >= 60 && Aggression < 80)
		{
			SetState(EState::Investigate);
		}
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
	UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(UAISense::GetSenseID(UAISense_Sight::StaticClass())));
	switch (newState)
	{
	case EState::Idle:
		break;
	case EState::Patrol:
		break;
	case EState::Investigate:
		break;
	case EState::Hunt:
		SightConfig->PeripheralVisionAngleDegrees = MonsterHuntVisionCone;
		PerceptionComponent->RequestStimuliListenerUpdate(); //CORRECT WAY OF DOING IT
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

void AMonster::AddAggression(float ValueToAdd)
{
	Aggression += ValueToAdd;
	Aggression = FMath::Clamp(Aggression, 0, 100);
	GetCharacterMovement()->MaxWalkSpeed = MonsterDefaultMoveSpeed * (Aggression/100 + 1);
}


