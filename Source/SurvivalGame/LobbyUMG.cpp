// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUMG.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"


bool ULobbyUMG::Initialize()
{
	Super::Initialize();
	if (NewGameButton != nullptr)
	{
		NewGameButton->OnClicked.AddDynamic(this, &ULobbyUMG::OnClickedNewGame);
	}
	return true;
}

void ULobbyUMG::OnClickedNewGame()
{
	UGameplayStatics::OpenLevel(this, FName("FirstPersonExampleMap"));
}