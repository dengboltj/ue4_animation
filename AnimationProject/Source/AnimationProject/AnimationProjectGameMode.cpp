// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AnimationProjectGameMode.h"
#include "AnimationProjectPlayerController.h"
#include "AnimationProjectCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAnimationProjectGameMode::AAnimationProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AAnimationProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted characterBlueBlueprint'/Game/TopDownCPP/Blueprints/RiftBP.RiftBP'
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/RiftBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAnimationProjectGameMode::BeginPlay()
{
	Super::BeginPlay();
}
