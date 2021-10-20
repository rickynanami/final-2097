// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"

#include "StunGrenade.generated.h"

class USoundBase;
class UNiagaraSystem;

UCLASS()
class SURVIVALGAME_API AStunGrenade : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStunGrenade();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Attribute")
		class UStaticMeshComponent* BaseMesh;

		UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;

	UPROPERTY(VisibleAnywhere)
	bool IsExplod = false;
	FTimerHandle TH_Explod;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		float DelayTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		USoundBase* ExplodeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attribute")
		UNiagaraSystem* ExplodeEffect;

	void OnExplode();

	void IsActiveTimer();

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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
		FString InteractText;

	UFUNCTION(BlueprintPure, BlueprintCallable)
		FORCEINLINE FString GetInteractText() { return InteractText; }

};
