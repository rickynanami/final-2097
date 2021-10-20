// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.generated.h"

/**
 *
 */
UCLASS()
class SURVIVALGAME_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual FRotator GetControlRotation() const override;
	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	void GenerateNewRandomLocation();

	//UFUNCTION()
		//void OnPawnDetected(const TArray<AActor*>& DetectedPawns);
	UFUNCTION()
		void OnsensesUpdate(AActor* Updatedactor, FAIStimulus Stimulus);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float SightRadius = 500;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float SightAge = 3;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float SightRadiusLose = SightRadius + 30;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float FieldOfView = 60;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		UAISenseConfig_Sight* SightConfiguration;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardData* AIBlackboard;
	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBlackboardComponent* BlackboardComponent;
	UPROPERTY(EditDefaultsOnly, Category = Blackboard)
		UBehaviorTree* BehaviorTree;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		UAISenseConfig_Hearing* HearingConfiguration;

	UFUNCTION()
		void OnHearNoise(TArray<AActor*> const& updated_actors);

	void ClearNosie();


	//敌人被地雷炸到，原地停止运动
	void BeginStun();
	FTimerHandle TimeHnadle_StunTime;
	void Stoptun();


};
