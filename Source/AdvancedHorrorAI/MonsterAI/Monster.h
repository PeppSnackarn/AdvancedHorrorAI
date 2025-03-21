// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"

class UAIPerceptionComponent;
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
	void HandleAggressionStates();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void HandleSenses(AActor* Actor,FAIStimulus Stimulus);
	void DecayAgression();
	
	protected:
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "Monster Info")
	EState CurrentState = EState::Idle;
	UPROPERTY(VisibleAnywhere, meta =(ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Monster Info")
	float Aggression = 0;
	UPROPERTY(EditAnywhere, Category = "Monster Config")
	float AggressionAddedPerSecond = 90;
	UPROPERTY(EditDefaultsOnly, Category = "Monster Info")
	UAIPerceptionComponent* PerceptionComponent = nullptr;
	UPROPERTY(VisibleAnywhere ,Category = "Monster Info")
	bool bCanSeePlayer = false;

	float TimeAtLastSeenPlayer;

	public:
	void SetState(EState newState);
	inline EState GetCurrentState() { return CurrentState; }
	inline void AddAggression(float ValueToAdd) { Aggression += ValueToAdd; Aggression = FMath::Clamp(Aggression, 0, 100); }
};
