// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABUserWidget.h"
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
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
	
};
