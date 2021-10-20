// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseUMG.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool UPauseUMG::Initialize()
{
	Super::Initialize();

	if (ResumeButton != nullptr)
	{
		ResumeButton->OnClicked.AddDynamic(this, &UPauseUMG::OnClickedResumeButton);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UPauseUMG::OnClickedExitButton);
	}

	return true;
}

void UPauseUMG::OnClickedResumeButton()
{
	UGameplayStatics::SetGamePaused(this, false);
	APlayerController* MyPc = GetWorld()->GetFirstPlayerController();
	MyPc->SetShowMouseCursor(false);
	RemoveFromViewport();
}

void UPauseUMG::OnClickedExitButton()
{
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"));
}