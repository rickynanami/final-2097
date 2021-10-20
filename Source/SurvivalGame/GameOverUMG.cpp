// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverUMG.h"
#include "Components//Button.h"
#include "Kismet/GameplayStatics.h"


bool UGameOverUMG::Initialize()
{
	Super::Initialize();

	if (ReplayButton != nullptr)
	{
		ReplayButton->OnClicked.AddDynamic(this, &UGameOverUMG::OnClickedReplayButton);
	}

	if (ExitButton != nullptr)
	{
		ExitButton->OnClicked.AddDynamic(this, &UGameOverUMG::OnClickedExitButton);
	}

	return true;
}

void UGameOverUMG::OnClickedReplayButton()
{
	UGameplayStatics::OpenLevel(this, FName("FirstPersonExampleMap"));
}

void UGameOverUMG::OnClickedExitButton()
{
	UGameplayStatics::OpenLevel(this, FName("LobbyMap"));
}