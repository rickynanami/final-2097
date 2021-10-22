// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = SightRadiusLose;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	//GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPawnDetected);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnsensesUpdate);

	//Auditory component initialization
	HearingConfiguration = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Configuration"));

	HearingConfiguration->HearingRange = 3000;

	HearingConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	HearingConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnHearNoise);
	GetPerceptionComponent()->ConfigureSense(*HearingConfiguration);

	TargetPlayer = nullptr;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	if (!AIBlackboard)
		return;
	if (!ensure(BehaviorTree))
		return;

	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);

	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	GenerateNewRandomLocation();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0, GetPawn()->GetActorRotation().Yaw, 0);
	}
	return FRotator(0, 0, 0);
}

void AEnemyAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	//Result.IsSuccess();
	/*if (NavigationSystem)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Navigation system set"));
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		MoveToLocation(ReturnLocation.Location);
	}*/
}

/*void AEnemyAIController::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
	for (auto actor : DetectedPawns)
	{
		APawn* TempPawn = Cast<APawn>(actor);
		if (TempPawn)
		{
			if (TempPawn->IsPlayerControlled())
			{
				if(TargetPlayer && TargetPlayer == TempPawn)
				{
					UE_LOG(LogTemp, Warning, TEXT("Player is already known"));
					TargetPlayer = nullptr;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Player has been sportted"));
					TargetPlayer = TempPawn;
				}
			}
		}
	}
}*/

void AEnemyAIController::OnsensesUpdate(AActor* Updatedactor, FAIStimulus Stimulus)
{
	APawn* TempPawn = Cast<APawn>(Updatedactor);
	if (TempPawn && TempPawn->IsPlayerControlled())
	{
		//Stimulus.StimulusLocation;
		if (Stimulus.WasSuccessfullySensed())
		{
			TargetPlayer = TempPawn;
			BlackboardComponent->SetValueAsBool("ChasePlayer", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("ChasePlayer");
		}

	}
}

void AEnemyAIController::OnHearNoise(TArray<AActor*> const& updated_actors)
{
	//Traverse the heard Actors, find the position of the object whose Tag is "Noise", and set the variables in the blackboard
	for (size_t x = 0; x < updated_actors.Num(); ++x)
	{
		FActorPerceptionBlueprintInfo info;
		GetPerceptionComponent()->GetActorsPerception(updated_actors[x], info);
		for (size_t k = 0; k < info.LastSensedStimuli.Num(); ++k)
		{
			FAIStimulus const stim = info.LastSensedStimuli[k];
			if (stim.Tag == "Noise")
			{
				BlackboardComponent->SetValueAsBool("IsInvestigating", stim.WasSuccessfullySensed());
				BlackboardComponent->SetValueAsVector("NoisePoint", stim.StimulusLocation);
			}
			else if (stim.Type.Name == "Default__AISense_Sight")
			{
				BlackboardComponent->SetValueAsBool("ChasePlayer", false);
			}
		}
	}
}

void AEnemyAIController::ClearNosie()
{
	BlackboardComponent->SetValueAsBool("IsInvestigating", false);
}

//AI stop activity
void AEnemyAIController::BeginStun()
{
	BlackboardComponent->SetValueAsBool("IsStun", true);
	GetWorld()->GetTimerManager().SetTimer(TimeHnadle_StunTime, this, &AEnemyAIController::Stoptun, 10.0, false);
}

void AEnemyAIController::Stoptun()
{
	BlackboardComponent->SetValueAsBool("IsStun", false);
}

//AI reactivity
void AEnemyAIController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Navigation system set"));
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

