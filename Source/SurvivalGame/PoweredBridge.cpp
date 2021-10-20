// Fill out your copyright notice in the Description page of Project Settings.


#include "PoweredBridge.h"
#include "PoweredKey.h"

// Sets default values
APoweredBridge::APoweredBridge()
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

}

// Called when the game starts or when spawned
void APoweredBridge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoweredBridge::Tick(float DeltaTime)
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

void APoweredBridge::Interact_Implementation()
{
	if (PowerKey->bIsPlaced)
	{
		bIsOpen = true;
		BaseMesh->SetMaterial(0, GlowMaterial);
	}
	else
	{
		InteractText = PlacedKeyInteractText;
	}
	
}

