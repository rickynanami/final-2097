// Fill out your copyright notice in the Description page of Project Settings.


#include "GenerateRandomLocationTask.h"

EBTNodeResult::Type UGenerateRandomLocationTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;
	if (!Component)
		return EBTNodeResult::Failed;

	AEnemyAIController* Mycontroller = Cast<AEnemyAIController>(Component->GetOwner());
	if (!Mycontroller)
		return EBTNodeResult::Failed;
	Mycontroller->GenerateNewRandomLocation();
	return EBTNodeResult::Succeeded;
}
