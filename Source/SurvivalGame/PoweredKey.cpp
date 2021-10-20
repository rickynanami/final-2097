// Fill out your copyright notice in the Description page of Project Settings.


#include "PoweredKey.h"
#include "Engine/StaticMesh.h"

// Sets default values
APoweredKey::APoweredKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	BaseMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void APoweredKey::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APoweredKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APoweredKey::Interact_Implementation()
{
	bIsPlaced = true;
	if (bIsPlaced)
	{
		BaseMesh->SetStaticMesh(NewMesh);
	}
}

