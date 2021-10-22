// Fill out your copyright notice in the Description page of Project Settings.


#include "StunGrenade.h"
#include "Kismet/GameplayStatics.h"
#include "SurvivalGameCharacter.h"
#include "../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnemyCharacter.h"
#include "EnemyAIController.h"

// Sets default values
AStunGrenade::AStunGrenade()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	BaseMesh->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BaseMesh;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);
	TriggerCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &AStunGrenade::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &AStunGrenade::OnOverlapEnd);

}

void AStunGrenade::OnExplode()
{
	// try and play the sound if specified
	if (ExplodeSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplodeSound, GetActorLocation());
	}
	if (ExplodeEffect != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplodeEffect, GetActorLocation());
		TriggerCapsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	Destroy();
}

void AStunGrenade::IsActiveTimer()
{
	if (IsExplod)
	{
		GetWorldTimerManager().SetTimer(TH_Explod, this, &AStunGrenade::OnExplode, DelayTime, false);
	}
}

void AStunGrenade::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AEnemyCharacter* OverlapEnemy = Cast<AEnemyCharacter>(OtherActor);
		if (OverlapEnemy)
		{   
			//Trigger the Stun function in AIController;
			AEnemyAIController* TempAIController = Cast<AEnemyAIController>(OverlapEnemy->GetController());
			TempAIController->BeginStun();
		}
	}
	
}

void AStunGrenade::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

// Called when the game starts or when spawned
void AStunGrenade::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStunGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStunGrenade::Interact_Implementation()
{
	ASurvivalGameCharacter* Player = Cast<ASurvivalGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	//pick up StunGrenade

	Player->IncreaseStunGrenade();
	Destroy();
}

