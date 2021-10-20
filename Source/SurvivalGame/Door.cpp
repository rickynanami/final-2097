// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "SurvivalGameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpen)
	{
		FVector TargetLocation = BaseMesh->GetComponentLocation();
		TargetLocation.Z = BaseZ + 330.0f;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), TargetLocation, 0.05f));
	}
	else
	{
		FVector TargetLocation = BaseMesh->GetComponentLocation();
		TargetLocation.Z = BaseZ;
		BaseMesh->SetRelativeLocation(FMath::Lerp(BaseMesh->GetComponentLocation(), TargetLocation, 0.05f));
	}

}

void ADoor::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pick Up"));
	bIsOpen = !bIsOpen;
}

