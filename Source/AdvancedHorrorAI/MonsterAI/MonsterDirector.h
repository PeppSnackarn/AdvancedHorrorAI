// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterDirector.generated.h"

class AAdvancedHorrorAICharacter;
class AMonster;

UCLASS()
class ADVANCEDHORRORAI_API AMonsterDirector : public AActor
{
	GENERATED_BODY()
	AMonsterDirector();

protected:
	AMonster* MonsterRef = nullptr;
	AAdvancedHorrorAICharacter* PlayerRef = nullptr;
private:
	float TimeAtLostSight = 0;
public:
	UPROPERTY(VisibleAnywhere)
	bool bIsHunting = false;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	bool EstablishedSightline();
	

};
