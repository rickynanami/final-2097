// Fill out your copyright notice in the Description page of Project Settings.


#include "Bridge.h"
#include "SurvivalGameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABridge::ABridge()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	BaseMesh->SetupAttachment(RootComponent);

	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	SwitchMesh->SetCollisionProfileName(TEXT("Trigger"));
	SwitchMesh->SetupAttachment(RootComponent);


	bIsOpen = false;
}

// Called when the game starts or when spawned
void ABridge::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABridge::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpen)
	{
		FVector TargetLocation = BaseMesh->GetComponentLocation();
		TargetLocation.Z = BaseZ + 330.0f;
		BaseMesh->SetWorldLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), TargetLocation, 0.05f));
	}
	else
	{
		FVector TargetLocation = BaseMesh->GetComponentLocation();
		TargetLocation.Z = BaseZ;
		BaseMesh->SetWorldLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), TargetLocation, 0.05f));
	}

}

void ABridge::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pick Up"));
	//BaseMesh->SetWorldLocation(GetActorLocation() + FVector(0, 0, 20.f));
	if (bIsNeedKey)
	{
		ASurvivalGameCharacter* MyPlayer = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		if (MyPlayer->KeyCount > 0)
		{
			MyPlayer->KeyCount--;
			bIsOpen = true;
			BaseMesh->SetMaterial(0,GlowMaterial);
		}
		else
		{
			InteractText = NoInteractText;
		}
	}
	else
	{
		bIsOpen = !bIsOpen;
		BaseMesh->SetMaterial(0, GlowMaterial);
	}

}

