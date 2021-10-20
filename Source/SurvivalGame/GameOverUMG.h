// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUMG.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UGameOverUMG : public UUserWidget
{
	GENERATED_BODY()

		virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ReplayButton;


	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

public:
	UFUNCTION()
		void OnClickedReplayButton();

	UFUNCTION()
		void OnClickedExitButton();
	
};
