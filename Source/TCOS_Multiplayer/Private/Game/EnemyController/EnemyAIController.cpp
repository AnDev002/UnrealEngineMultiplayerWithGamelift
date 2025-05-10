// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/EnemyController/EnemyAIController.h"

AEnemyAIController::AEnemyAIController()
{
	bAttachToPawn = true;
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset)
	{
		RunBehaviorTree(BehaviorTreeAsset);
	}
}
