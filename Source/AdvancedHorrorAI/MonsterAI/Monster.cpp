// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"
#include "AIController.h"
#include "MonsterDirector.h"
#include "AdvancedHorrorAI/AdvancedHorrorAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	ensureAlwaysMsgf(FakeQueryActorClass, TEXT("FakeQueryActor not set; EQS will not function!")); // ensure that EQS will function
	if(FakeQueryActorClass)
	{
		FakeQueryActor = GetWorld()->SpawnActor(FakeQueryActorClass);
	}
	if (GetController())
		BlackboardComponent = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if (BlackboardComponent)
	{
		BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
		BlackboardComponent->SetValueAsObject("PlayerRef", GetWorld()->GetFirstPlayerController());
	}
	SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComponent->GetSenseConfig(UAISense::GetSenseID(UAISense_Sight::StaticClass())));
	DirectorRef = Cast<AMonsterDirector>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonsterDirector::StaticClass()));
	MonsterDefaultMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	MonsterDefaultVisionCone = SightConfig->PeripheralVisionAngleDegrees;
}

void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if(bCanSeePlayer)
	{
		AddAggression(AggressionAddedPerSecond * DeltaTime);
		APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();

		BlackboardComponent->SetValueAsVector("LastPlayerLocation", Player->GetActorLocation()); // Ugly solution to always have last player location available
		// since monster is having hard time tracking player, should find a way to make sure that the player is in sight of the monster or the director lets the monster cheat.

		FVector Velocity = Player->GetVelocity();
		if(Velocity.IsNearlyZero())
			Velocity = Player->GetActorForwardVector();
		BlackboardComponent->SetValueAsRotator("LastPlayerRotation", Velocity.Rotation());
		
		if(Aggression >= 60 && Aggression < 80)
		{
			SetState(EState::Investigate);
		}
		else if(Aggression >= 80) 
		{
			SetState(EState::Hunt);
		}
	}
	else
	{
		if(CurrentState == EState::Idle)
		{
			if(TimeAtLastSense + AggressionDecayCooldown <= GetWorld()->GetTime().GetRealTimeSeconds() && bDecayAgression)
				AddAggression(-AggressionDecayPerSecond * DeltaTime);
		}
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
		AddAggression(30);
		BlackboardComponent->SetValueAsVector("LastHeardLocation", Actor->GetActorLocation());
		if(Aggression >= 60 && Aggression < 80)
		{
			SetState(EState::Investigate);
		}
		if(CurrentState == EState::Idle)
		{
			SetState(EState::StopListen);
		}
	}
	TimeAtLastSense = GetWorld()->GetTime().GetRealTimeSeconds();
}

void AMonster::SetState(EState newState)
{
	CurrentState = newState;
	BlackboardComponent->SetValueAsEnum("CurrentState", static_cast<uint8>(CurrentState));
	switch (newState)
	{
	case EState::Idle:
		SightConfig->PeripheralVisionAngleDegrees = MonsterDefaultVisionCone;
		break;
	case EState::StopListen:
		break;
	case EState::Investigate:
		break;
	case EState::Hunt:
		SightConfig->PeripheralVisionAngleDegrees = MonsterHuntVisionCone;
		if(DirectorRef)
			DirectorRef->bIsHunting = true;
		break;
	case EState::Leave:
		break;
	}
	PerceptionComponent->RequestStimuliListenerUpdate(); // Updates sense-configs
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


