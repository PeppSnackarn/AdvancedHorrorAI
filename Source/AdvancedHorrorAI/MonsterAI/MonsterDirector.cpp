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
			if(TimeAtLostSight + 5 <= GetWorld()->GetTime().GetRealTimeSeconds())
			{
				MonsterRef->SetState(EState::Investigate);
				bIsHunting = false;
			}
		}
	}
}

bool AMonsterDirector::EstablishedSightline()
{
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	ActorLineTraceSingle(HitResult, MonsterRef->GetActorLocation(), PlayerRef->GetActorLocation(), ECC_Visibility, CollisionQueryParams);
	if(HitResult.GetActor() == PlayerRef)
	{
		return true;
	}
	TimeAtLostSight = GetWorld()->GetTime().GetRealTimeSeconds(); // doesnt work, gets rewritten every frame
	return false;
}
