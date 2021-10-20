// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "SurvivalGameCharacter.h"

// Sets default values
AHealthPickup::AHealthPickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

void AHealthPickup::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pick Up"));
	//BaseMesh->SetWorldLocation(GetActorLocation() + FVector(0, 0, 20.f));

	ASurvivalGameCharacter* MyPlayer = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	switch (ItemType)
	{
	case  EItemType::Food:
		MyPlayer->IncreaseFood(Food);
		break;
	case EItemType::Water:
		MyPlayer->IncreaseWater(Water);
		break;
	case EItemType::Health:
		MyPlayer->IncreaseHealth(Health);
		break;
	}
	Destroy();
}

// Called when the game starts or when spawned
void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHealthPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

