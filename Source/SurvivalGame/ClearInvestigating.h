// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ClearInvestigating.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UClearInvestigating : public UBTTaskNode
{
	// AI IGNORE NOISE'S TASK
	GENERATED_BODY()
		EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
