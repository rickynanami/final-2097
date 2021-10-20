// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUMG.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ULobbyUMG : public UUserWidget
{
	GENERATED_BODY()

		virtual bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
		class UButton* NewGameButton;

public:
	UFUNCTION()
		void OnClickedNewGame();
	
};
