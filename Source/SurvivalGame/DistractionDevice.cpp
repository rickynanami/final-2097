// Fill out your copyright notice in the Description page of Project Settings.


#include "DistractionDevice.h"

#include "Perception/AISense_Hearing.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADistractionDevice::ADistractionDevice()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;

}

// Called when the game starts or when spawned
void ADistractionDevice::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADistractionDevice::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADistractionDevice::Interact_Implementation()
{
	// When player interaction pushing button E , make noise , to trigger AI's auditory sense
	UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1.0f, this, 0.0f, "Noise");
	if (NoiseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, NoiseSound, GetActorLocation());
	}
}

