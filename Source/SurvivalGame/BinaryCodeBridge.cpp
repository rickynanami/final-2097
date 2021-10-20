// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryCodeBridge.h"
#include "BinaryCodeKey.h"

// Sets default values
ABinaryCodeBridge::ABinaryCodeBridge()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ABinaryCodeBridge::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABinaryCodeBridge::Tick(float DeltaTime)
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

void ABinaryCodeBridge::CheckBinaryCodeKeysState()
{
	int32 ONKeyCont = 0;
	for (ABinaryCodeKey* K : BinaryCodeKeys)
	{
		if (K->bIsOn)
		{
			ONKeyCont ++;
		}
	}

	if (ONKeyCont >= 3)
	{
		bIsOpen = true;
		if (GlowMaterial)
		{
			BaseMesh->SetMaterial(0, GlowMaterial);
		}
	}
}

