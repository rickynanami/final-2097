// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseUMG.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UPauseUMG : public UUserWidget
{
	GENERATED_BODY()

		virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;


	UPROPERTY(meta = (BindWidget))
		class UButton* ExitButton;

public:
	UFUNCTION()
		void OnClickedResumeButton();

	UFUNCTION()
		void OnClickedExitButton();
	
};
