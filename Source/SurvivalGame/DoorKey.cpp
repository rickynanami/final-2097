// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorKey.h"
#include "SurvivalGameCharacter.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ADoorKey::ADoorKey()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;
}

// Called when the game starts or when spawned
void ADoorKey::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADoorKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorKey::Interact_Implementation()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Pick Up"));

	ASurvivalGameCharacter* MyPlayer = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MyPlayer->IncreaseKey();

	Destroy();
}

