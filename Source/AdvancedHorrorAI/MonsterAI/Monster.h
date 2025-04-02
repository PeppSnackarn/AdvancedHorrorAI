// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
UENUM()
enum class ELastSensedSense : uint8
{
	None,
	Sight,
	Hearing
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
	UFUNCTION()
	void HandleSenses(AActor* Actor,FAIStimulus Stimulus);
	bool ShouldDecayAggression();
	
	protected:
	UBlackboardComponent* BlackboardComponent;

	UPROPERTY(VisibleAnywhere, Category = "Monster Info")
	EState CurrentState = EState::Idle;
	UPROPERTY(VisibleAnywhere, Category = "Monster Info")
	ELastSensedSense LastSensedSense = ELastSensedSense::None;
	UPROPERTY(VisibleAnywhere, meta =(ClampMin = "0.0", ClampMax = "100.0", UIMin = "0.0", UIMax = "100.0"), Category = "Monster Info")
	float Aggression = 0;
	UPROPERTY(VisibleAnywhere ,Category = "Monster Info")
	float CurrentMoveSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Monster Info")
	UAIPerceptionComponent* PerceptionComponent = nullptr;
	UPROPERTY(VisibleAnywhere ,Category = "Monster Info")
	bool bCanSeePlayer = false;
	
	UPROPERTY(EditAnywhere, Category = "Monster Config")
	float AggressionAddedPerSecond = 90;
	UPROPERTY(EditAnywhere, Category = "Monster Config")
	bool bDecayAgression = true;
	UPROPERTY(EditAnywhere, Category = "Monster Config")
	float AggressionDecayPerSecond = 40;
	UPROPERTY(EditAnywhere, Category = "Monster Config")
	float AggressionDecayCooldown = 5;

	float TimeAtLastSensedPlayer;
	float MonsterDefaultMoveSpeed;
	
	public:
	void SetState(EState newState);
	void SetLastSenseSensed(ELastSensedSense newSense);
	void AddAggression(float ValueToAdd);
};
