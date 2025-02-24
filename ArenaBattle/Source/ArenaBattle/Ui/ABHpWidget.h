// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
#include "GameData/ABCharacterStat.h"
#include "ABHpWidget.generated.h"

/**
 * 
 */
UCLASS()
class ARENABATTLE_API UABHpWidget : public UABUserWidget
{
	GENERATED_BODY()

public:
	UABHpWidget(const FObjectInitializer& ObjentIntializer);

protected:
	virtual void NativeConstruct() override;
public:
	void UpdateStat(const FABCharacterStat& BaseStat, const FABCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	FString GetHpStatText();


protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<class UTextBlock> HpStat;

	UPROPERTY()
	float CurrentHp;

	UPROPERTY()
	float MaxHp;
	
};
