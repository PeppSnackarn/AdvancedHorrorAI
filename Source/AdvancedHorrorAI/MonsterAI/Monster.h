// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

class UBlackboardComponent;

UENUM()
enum class EState : uint8
{
	Idle,
	Patrol,
	Investigate,
	Hunt,
	Leave
};
UCLASS()
class ADVANCEDHORRORAI_API AMonster : public ACharacter
{
	GENERATED_BODY()

public:
	AMonster();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	protected:
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "Monster")
	EState CurrentState = EState::Idle;
	UPROPERTY(VisibleAnywhere, meta =(ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Monster")
	float Aggression = 0;

	public:
	void SetState(EState newState);
	inline EState GetCurrentState() { return CurrentState; }
	inline void AddAggression(float ValueToAdd) { Aggression += ValueToAdd; Aggression = FMath::Clamp(Aggression, 0, 100); }
};