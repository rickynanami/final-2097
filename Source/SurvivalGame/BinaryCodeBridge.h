// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BinaryCodeBridge.generated.h"


class ABinaryCodeKey;
UCLASS()
class SURVIVALGAME_API ABinaryCodeBridge : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABinaryCodeBridge();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		class UMaterialInterface* GlowMaterial;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

		bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		float BaseZ = 330.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		TArray<ABinaryCodeKey*> BinaryCodeKeys;

	FString InteractText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString YesInteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString NoInteractText;

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE FString GetInteractText() { return InteractText; }

	void CheckBinaryCodeKeysState();
};
