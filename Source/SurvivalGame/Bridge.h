// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Bridge.generated.h"

UCLASS()
class SURVIVALGAME_API ABridge : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABridge();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* SwitchMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Interact")
		class UMaterialInterface * GlowMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		bool bIsNeedKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float BaseZ = 330.0f;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;
	FString InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString YesInteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString NoInteractText;

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE FString GetInteractText() { return InteractText; }

};
