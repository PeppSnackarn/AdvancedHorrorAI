// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterDirector.h"

#include "Monster.h"
#include "AdvancedHorrorAI/AdvancedHorrorAICharacter.h"
#include "Kismet/GameplayStatics.h"


AMonsterDirector::AMonsterDirector()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMonsterDirector::BeginPlay()
{
	Super::BeginPlay();
	MonsterRef = Cast<AMonster>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonster::StaticClass()));
	PlayerRef = Cast<AAdvancedHorrorAICharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

void AMonsterDirector::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(bIsHunting)
	{
		if(!EstablishedSightline())
		{
			if(TimeAtLostSight >= 0 && TimeAtLostSight + LostSightTime <= GetWorld()->GetTime().GetRealTimeSeconds())
			{
				MonsterRef->SetState(EState::Investigate);
				UE_LOG(LogTemp, Log, TEXT("Set state to investigate."))
				bIsHunting = false;
			}
		}
	}
}

bool AMonsterDirector::EstablishedSightline()
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	GetWorld()->LineTraceSingleByChannel(HitResult, MonsterRef->GetActorLocation(), PlayerRef->GetActorLocation(), ECC_Visibility, CollisionQueryParams);
	bool bCanSeePlayer = HitResult.GetActor() == PlayerRef;
	if(bHadSightLastFrame && !bCanSeePlayer)
	{
		TimeAtLostSight = GetWorld()->GetTime().GetRealTimeSeconds(); 
	}
	bHadSightLastFrame = bCanSeePlayer;
	return bCanSeePlayer;
}
