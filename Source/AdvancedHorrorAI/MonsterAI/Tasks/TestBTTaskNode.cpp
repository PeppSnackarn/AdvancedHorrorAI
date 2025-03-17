// Fill out your copyright notice in the Description page of Project Settings.


#include "TestBTTaskNode.h"

UTestBTTaskNode::UTestBTTaskNode()
{
	NodeName = "Test Node";
}

EBTNodeResult::Type UTestBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Log, TEXT("Task was fired."))
	return EBTNodeResult::Succeeded;
}
