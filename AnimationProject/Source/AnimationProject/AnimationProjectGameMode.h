// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AnimationProjectGameMode.generated.h"

UCLASS(minimalapi)
class AAnimationProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAnimationProjectGameMode();

protected:
	virtual void BeginPlay() override;
};



