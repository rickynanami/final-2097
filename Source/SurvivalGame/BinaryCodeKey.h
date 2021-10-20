// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "BinaryCodeKey.generated.h"

class ABinaryCodeBridge;

UCLASS()
class SURVIVALGAME_API ABinaryCodeKey : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABinaryCodeKey();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interact")
		class UStaticMeshComponent* BaseMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Interact")
		bool bIsOn;

	class UMaterialInstanceDynamic* MaterialInstance;
	UPROPERTY(EditAnywhere, Category = "Interact")
		class ABinaryCodeBridge* BinaryBridge;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact();
	virtual void Interact_Implementation() override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString InteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString OnInteractText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interact")
		FString OffInteractText;

	UFUNCTION(BlueprintPure, BlueprintCallable)
	FString GetInteractText();

};
