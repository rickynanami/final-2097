// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "PoweredBridge.generated.h"

UCLASS()
class SURVIVALGAME_API APoweredBridge : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APoweredBridge();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* SwitchMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		class UMaterialInterface* GlowMaterial;

	UPROPERTY(EditAnywhere,Category = "Interact")
		class APoweredKey* PowerKey;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float BaseZ = 330.0f;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString PlacedKeyInteractText;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString SwitchFuseInteractText;*/

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE FString GetInteractText() { return InteractText; }

};
