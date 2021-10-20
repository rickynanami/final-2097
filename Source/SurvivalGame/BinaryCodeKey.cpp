// Fill out your copyright notice in the Description page of Project Settings.


#include "BinaryCodeKey.h"
#include "BinaryCodeBridge.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ABinaryCodeKey::ABinaryCodeKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	BaseMesh->SetupAttachment(RootComponent);



}

// Called when the game starts or when spawned
void ABinaryCodeKey::BeginPlay()
{
	Super::BeginPlay();

	MaterialInstance = BaseMesh->CreateDynamicMaterialInstance(0, BaseMesh->GetMaterial(0));

	if (bIsOn)
	{
		MaterialInstance->SetVectorParameterValue("Colour",FLinearColor(0,1,0));
	}
	else
	{
		MaterialInstance->SetVectorParameterValue("Colour", FLinearColor(1, 1, 0));
	}
	
}

// Called every frame
void ABinaryCodeKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABinaryCodeKey::Interact_Implementation()
{
	bIsOn = !bIsOn;

	if (bIsOn)
	{
		MaterialInstance->SetVectorParameterValue("Colour", FLinearColor(0, 1, 0));
	}
	else
	{
		MaterialInstance->SetVectorParameterValue("Colour", FLinearColor(1, 1, 0));
	}
	if (BinaryBridge)
	{
		BinaryBridge->CheckBinaryCodeKeysState();
	}
}

FString ABinaryCodeKey::GetInteractText()
{
	 if (bIsOn)
	{
		InteractText = OnInteractText;
	}
	else
	{
		InteractText = OffInteractText;

	}
	return InteractText;
}

