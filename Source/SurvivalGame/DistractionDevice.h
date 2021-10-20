// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "DistractionDevice.generated.h"

class USoundBase;
UCLASS()
class SURVIVALGAME_API ADistractionDevice : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADistractionDevice();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item attribute")
		class UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		USoundBase* NoiseSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString InteractText;

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE FString GetInteractText() { return InteractText; }
};
