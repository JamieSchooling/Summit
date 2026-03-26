// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;
class UHealthComponent;

/**
 * 
 */
UCLASS()
class SUMMIT_API UHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> HealthTextBlock;
	
	UFUNCTION(BlueprintCallable)
	void SetTargetHealthComponent(UHealthComponent* NewHealthComponent);
private:
	TWeakObjectPtr<UHealthComponent> TargetHealthComponent; 
	
	UFUNCTION()
	void OnHealthChanged(float NewHealth, float MaxHealth);
};
