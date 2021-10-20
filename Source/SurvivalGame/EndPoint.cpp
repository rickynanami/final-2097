// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"
#include "GameOverUMG.h"
#include "SurvivalGameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEndPoint::AEndPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	VisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleComponent"));
	VisibleComponent->SetupAttachment(RootComponent);

	EndDistance = 200.0f;
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Player)
	{
		FVector TargetDirection = GetActorLocation() - Player->GetActorLocation();
		if (TargetDirection.Size() < EndDistance)
		{
			UGameOverUMG* GameOverUMG = CreateWidget<UGameOverUMG>(GetWorld(), LoadClass<UGameOverUMG>(this, TEXT("WidgetBlueprint'/Game/UMG/UMG_GameOver.UMG_GameOver_C'")));
			GameOverUMG->AddToViewport();
			APlayerController* MyPlayerController = GetWorld()->GetFirstPlayerController();
			MyPlayerController->SetShowMouseCursor(true);
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}

