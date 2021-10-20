// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class AStunGrenade;
class APostProcessVolume;

UCLASS(config = Game)
class ASurvivalGameCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, Category = "Trigger Capsule")
		class UCapsuleComponent* TriggerCapsule;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* FP_MuzzleLocation;

	/** Gun mesh: VR view (attached to the VR controller directly, no arm, just the actual gun) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* VR_Gun;

	/** Location on VR gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USceneComponent* VR_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FirstPersonCameraComponent;

	/** Motion controller (right hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* R_MotionController;

	/** Motion controller (left hand) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UMotionControllerComponent* L_MotionController;

public:
	ASurvivalGameCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASurvivalGameProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		uint8 bUsingMotionControllers : 1;

protected:

	/** Fires a projectile. */
	void OnFire();

	/** Resets HMD orientation and position in VR. */
	void OnResetVR();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	struct TouchData
	{
		TouchData() { bIsPressed = false; Location = FVector::ZeroVector; }
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


	//Content
	UPROPERTY(BlueprintReadWrite,Category = "Attributes")
		int32 KeyCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float MaxHealth = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
		float CurrentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float MaxFood = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
		float CurrentFood;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
		float MaxWater = 100.0f;
	UPROPERTY(BlueprintReadWrite, Category = "Attributes")
		float CurrentWater;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FTimerHandle TH_MinusFood;
	void MinusFood();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoodAttributes")
		float MinusFoodRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FoodAttributes")
		float MinusFoodValue = 2.0f;
	void IncreaseFood(float FoodValue);

	FTimerHandle TH_MinusWater;
	void MinusWater();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterAttributes")
		float MinusWaterRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterAttributes")
		float MinusWaterValue = 2.0f;
	void IncreaseWater(float WaterValue);

	UPROPERTY(BlueprintReadWrite)
		class AActor* InteractableActor;

	FTimerHandle TH_MinusHealth;
	void MinusHealth();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthAttributes")
		float MinusHealthRate = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthAttributes")
		float MinusHealthValue = 2.0f;
	void IncreaseHealth(float HealthValue);

	//
	UPROPERTY(BlueprintReadWrite,Category = "Attributes")
	int32 StunGrenadeCount;

	UPROPERTY(EditAnywhere,Category = "StunGrenadeClass")
	TSubclassOf<AStunGrenade> StunGrenadeClass;
	void IncreaseStunGrenade();
	void DecreaseStunGrenade();
	void OnStunGrenade();

	
	APostProcessVolume* ppv;

	UFUNCTION(BlueprintCallable)
	void BeginGame();

	void CallInteract();
	void IncreaseKey();
	void PauseGame();
	void RestartGame();
	void QuitGame();
};

