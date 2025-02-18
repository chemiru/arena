// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "Engine/DataAsset.h"
#include "ABCharacterControlData.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABCharacterControlData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UABCharacterControlData();

	UPROPERTY(EditAnywhere, Category = Pawn)
	bool bUseControllerRotationYaw = false;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	bool bOrientRotationToMovement = false;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	bool bUseControllerDesiredRotation = false;

	UPROPERTY(EditAnywhere, Category = CharacterMovement)
	FRotator RotationRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<class UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	float TargetArmLength;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bUsePawnControlRotation = false;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritPitch = false;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritYaw = false;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bInheritRoll = false;

	UPROPERTY(EditAnywhere, Category = SpringArm)
	bool bDoCollisionTest = false;


	
};
