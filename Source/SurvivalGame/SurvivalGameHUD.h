// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SurvivalGameHUD.generated.h"

UCLASS()
class ASurvivalGameHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASurvivalGameHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

