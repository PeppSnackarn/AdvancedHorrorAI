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
	UPROPERTY(VisibleAnywhere)
	float TimeAtLostSight = -1; // default number to ensure that AI can't "lose sight" on scene start
	UPROPERTY(VisibleAnywhere)
	bool bHadSightLastFrame = false;
	UPROPERTY(EditAnywhere)
	float LostSightTime = 3;
public:
	UPROPERTY(VisibleAnywhere, Category = "Director Values")
	bool bIsHunting = false;
protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	bool EstablishedSightline();
	

};
