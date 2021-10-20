// Copyright Epic Games, Inc. All Rights Reserved.

#include "SurvivalGameCharacter.h"
#include "SurvivalGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Components/CapsuleComponent.h"
#include "Interactable.h"
#include "TimerManager.h"
#include "PauseUMG.h"
#include "GameOverUMG.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Math/UnrealMathUtility.h"
#include "Bridge.h"
#include "EnemyCharacter.h"
#include "StunGrenade.h"
#include "Engine/Scene.h"
#include "Engine/PostProcessVolume.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ASurvivalGameCharacter

ASurvivalGameCharacter::ASurvivalGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;

	TriggerCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Trigger Capsule"));
	TriggerCapsule->InitCapsuleSize(55.0f, 96.0f);
	TriggerCapsule->SetCollisionProfileName(TEXT("Trigger"));
	TriggerCapsule->SetupAttachment(RootComponent);

	TriggerCapsule->OnComponentBeginOverlap.AddDynamic(this, &ASurvivalGameCharacter::OnOverlapBegin);
	TriggerCapsule->OnComponentEndOverlap.AddDynamic(this, &ASurvivalGameCharacter::OnOverlapEnd);
}

void ASurvivalGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	CurrentFood = MaxFood;
	CurrentHealth = MaxHealth;
	CurrentWater = MaxWater;

	KeyCount = 0;

	//获取Tag为Post的后期处理盒子
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(this, FName("Post"), Actors);
	if (Actors.Num() > 0)
	{
		ppv = Cast<APostProcessVolume>(Actors[0]);
	}

}
//////////////////////////////////////////////////////////////////////////
// Input

void ASurvivalGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASurvivalGameCharacter::OnFire);

	PlayerInputComponent->BindAction("OnStunGrenade", IE_Pressed, this, &ASurvivalGameCharacter::OnStunGrenade);

	// Bind E event
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASurvivalGameCharacter::CallInteract);

	PlayerInputComponent->BindAction("PauseGame", IE_Pressed, this, &ASurvivalGameCharacter::PauseGame);
	PlayerInputComponent->BindAction("RestartGame", IE_Pressed, this, &ASurvivalGameCharacter::RestartGame);
	PlayerInputComponent->BindAction("QuitGame", IE_Pressed, this, &ASurvivalGameCharacter::QuitGame);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ASurvivalGameCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ASurvivalGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASurvivalGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASurvivalGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASurvivalGameCharacter::LookUpAtRate);
}

void ASurvivalGameCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ASurvivalGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<ASurvivalGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ASurvivalGameCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ASurvivalGameCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ASurvivalGameCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ASurvivalGameCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void ASurvivalGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ASurvivalGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ASurvivalGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASurvivalGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ASurvivalGameCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ASurvivalGameCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ASurvivalGameCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ASurvivalGameCharacter::TouchUpdate);
		return true;
	}

	return false;
}

void ASurvivalGameCharacter::OnOverlapBegin(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemyCharacter* EnemyActor = Cast<AEnemyCharacter>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherComp && !EnemyActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("OverlapBegin"));
		//Implements Interface

		InteractableActor = OtherActor;
		ABridge* Bridge = Cast<ABridge>(OtherActor);
		if (Bridge)
		{
			Bridge->InteractText = Bridge->YesInteractText;
		}
	}
}

void ASurvivalGameCharacter::OnOverlapEnd(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEnemyCharacter* EnemyActor = Cast<AEnemyCharacter>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherComp && !EnemyActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Overlap End"));

		InteractableActor = nullptr;
	}
}

void ASurvivalGameCharacter::MinusFood()
{
	float TempFood = CurrentFood - MinusFoodValue;
	CurrentFood = FMath::Clamp<float>(TempFood, 0, MaxFood);
	if (CurrentFood <= 0)
	{
		GetWorldTimerManager().ClearTimer(TH_MinusFood);
		if (GetWorldTimerManager().IsTimerActive(TH_MinusHealth))
		{
			MinusHealthValue = MinusHealthValue * 2;
		}
		else
		{
			GetWorldTimerManager().SetTimer(TH_MinusHealth, this, &ASurvivalGameCharacter::MinusHealth, MinusHealthRate, true);
		}
	}
}

void ASurvivalGameCharacter::IncreaseFood(float FoodValue)
{
	float TempFood = CurrentFood + FoodValue;
	CurrentFood = FMath::Clamp<float>(TempFood, 0, MaxFood);

	if (CurrentFood > 0 && TH_MinusHealth.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TH_MinusHealth);
		if (!TH_MinusFood.IsValid())
		{
			GetWorldTimerManager().SetTimer(TH_MinusFood, this, &ASurvivalGameCharacter::MinusFood, MinusFoodRate, true);
		}
	}
}

void ASurvivalGameCharacter::MinusWater()
{
	float TempWater = CurrentWater - MinusWaterValue;
	CurrentWater = FMath::Clamp<float>(TempWater, 0, MaxWater);

	if (CurrentWater <= 0)
	{
		GetWorldTimerManager().ClearTimer(TH_MinusWater);
		if (GetWorldTimerManager().IsTimerActive(TH_MinusHealth))
		{
			MinusHealthValue = MinusHealthValue * 2;
		}
		else
		{
			GetWorldTimerManager().SetTimer(TH_MinusHealth, this, &ASurvivalGameCharacter::MinusHealth, MinusHealthRate, true);
		}
	}
}

void ASurvivalGameCharacter::IncreaseWater(float WaterValue)
{

	float TempWater = CurrentWater + WaterValue;
	CurrentWater = FMath::Clamp<float>(TempWater, 0, MaxWater);
	if (CurrentWater > 0 && TH_MinusHealth.IsValid())
	{
		GetWorldTimerManager().ClearTimer(TH_MinusHealth);

		if (!TH_MinusWater.IsValid())
		{
			GetWorldTimerManager().SetTimer(TH_MinusWater, this, &ASurvivalGameCharacter::MinusWater, MinusWaterRate, true);
		}

	}
}

void ASurvivalGameCharacter::MinusHealth()
{

	float TempHealth = CurrentHealth - MinusHealthValue;
	CurrentHealth = FMath::Clamp<float>(TempHealth, 0, MaxHealth);

	if (CurrentHealth < 20)
	{
		if (ppv)
		{
			FPostProcessSettings& PostProcessSettings = ppv->Settings;
			PostProcessSettings.ColorSaturation = FVector4(0.f, 1.f, 0.f, 0.f);
		}
	}

	if (CurrentHealth <= 0)
	{
		UGameOverUMG* GameOverUMG = CreateWidget<UGameOverUMG>(GetWorld(), LoadClass<UGameOverUMG>(this, TEXT("WidgetBlueprint'/Game/UMG/UMG_GameOver.UMG_GameOver_C'")));
		GameOverUMG->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		APlayerController* MyPc = GetWorld()->GetFirstPlayerController();
		MyPc->SetShowMouseCursor(true);
	}
}

void ASurvivalGameCharacter::IncreaseHealth(float HealthValue)
{
	float TempHealth = CurrentHealth + HealthValue;
	CurrentHealth = FMath::Clamp<float>(TempHealth, 0, MaxHealth);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::FromInt(HealthValue));

	InteractableActor = nullptr;
}

void ASurvivalGameCharacter::IncreaseStunGrenade()
{
	StunGrenadeCount++;
}

void ASurvivalGameCharacter::DecreaseStunGrenade()
{
	StunGrenadeCount++;
}

void ASurvivalGameCharacter::OnStunGrenade()
{
	// try and fire a projectile
	if (StunGrenadeClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (StunGrenadeCount > 0)
			{
				const FRotator SpawnRotation = GetActorRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = GetActorLocation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// spawn the projectile at the muzzle
				AStunGrenade* TempAstunGrenade = World->SpawnActor<AStunGrenade>(StunGrenadeClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				TempAstunGrenade->IsExplod = true;
				TempAstunGrenade->IsActiveTimer();
				StunGrenadeCount--;
			}
		}
	}
}

void ASurvivalGameCharacter::BeginGame()
{
	GetWorldTimerManager().SetTimer(TH_MinusFood, this, &ASurvivalGameCharacter::MinusFood, MinusFoodRate, true);
	GetWorldTimerManager().SetTimer(TH_MinusWater, this, &ASurvivalGameCharacter::MinusWater, MinusWaterRate, true);
}

void ASurvivalGameCharacter::CallInteract()
{
	if (InteractableActor && InteractableActor->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		IInteractable::Execute_Interact(InteractableActor);
	}

}

void ASurvivalGameCharacter::IncreaseKey()
{
	KeyCount++;
}

void ASurvivalGameCharacter::PauseGame()
{
	UPauseUMG* PauseUMG = CreateWidget<UPauseUMG>(GetWorld(), LoadClass<UPauseUMG>(this, TEXT("WidgetBlueprint'/Game/UMG/UMG_Pause.UMG_Pause_C'")));

	PauseUMG->AddToViewport();
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);

	UGameplayStatics::SetGamePaused(this, true);
}

void ASurvivalGameCharacter::RestartGame()
{
	UGameplayStatics::OpenLevel(this, FName("FirstPersonExampleMap"));
}

void ASurvivalGameCharacter::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, true);
}
