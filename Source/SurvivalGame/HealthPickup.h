// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"

#include "HealthPickup.generated.h"


UENUM(BlueprintType)
enum class EItemType : uint8
{
	Food,
	Water,
	Health
};

UCLASS()
class SURVIVALGAME_API AHealthPickup : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHealthPickup();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Interact")
		EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float Health = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float Water = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float Food = 20.0f;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Interact")
		FString InteractText;

	UFUNCTION(BlueprintPure,BlueprintCallable)
	FORCEINLINE FString GetInteractText(){return InteractText;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

