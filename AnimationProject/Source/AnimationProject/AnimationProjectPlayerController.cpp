// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "AnimationProjectPlayerController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "AnimationProjectCharacter.h"
#include "Engine/World.h"

AAnimationProjectPlayerController::AAnimationProjectPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void AAnimationProjectPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	// keep updating the destination every tick while desired
	if (bMoveToMouseCursor)
	{
		MoveToMouseCursor();
	}
}

void AAnimationProjectPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("SetDestination", IE_Pressed, this, &AAnimationProjectPlayerController::OnSetDestinationPressed);
	InputComponent->BindAction("SetDestination", IE_Released, this, &AAnimationProjectPlayerController::OnSetDestinationReleased);

	InputComponent->BindAction("MouseRightClick", IE_Released, this, &AAnimationProjectPlayerController::OnMouseRightClick);


	// support touch devices 
	InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AAnimationProjectPlayerController::MoveToTouchLocation);
	InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AAnimationProjectPlayerController::MoveToTouchLocation);

	InputComponent->BindAction("ResetVR", IE_Pressed, this, &AAnimationProjectPlayerController::OnResetVR);
}

void AAnimationProjectPlayerController::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAnimationProjectPlayerController::MoveToMouseCursor()
{
	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		if (AAnimationProjectCharacter* MyPawn = Cast<AAnimationProjectCharacter>(GetPawn()))
		{
			if (MyPawn->GetCursorToWorld())
			{
				UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, MyPawn->GetCursorToWorld()->GetComponentLocation());
			}
		}
	}
	else
	{
		// Trace to see what is under the mouse cursor
		FHitResult Hit;
		GetHitResultUnderCursor(ECC_Visibility, false, Hit);

		if (Hit.bBlockingHit)
		{
			// We hit something, move there
			SetNewMoveDestination(Hit.ImpactPoint);
		}
	}
}

void AAnimationProjectPlayerController::MoveToTouchLocation(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	FVector2D ScreenSpaceLocation(Location);

	// Trace to see what is under the touch location
	FHitResult HitResult;
	GetHitResultAtScreenPosition(ScreenSpaceLocation, CurrentClickTraceChannel, true, HitResult);
	if (HitResult.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(HitResult.ImpactPoint);
	}
}

void AAnimationProjectPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void AAnimationProjectPlayerController::OnSetDestinationPressed()
{
	// set flag to keep updating destination until released
	bMoveToMouseCursor = true;
}

void AAnimationProjectPlayerController::OnSetDestinationReleased()
{
	// clear flag to indicate we should stop updating the destination
	bMoveToMouseCursor = false;
}

//鼠标右键点击播放动作
void AAnimationProjectPlayerController::OnMouseRightClick()
{
	// set flag to keep updating destination until released
	UAnimSequence* AnimSeq = LoadObject<UAnimSequence>(NULL, TEXT("AnimSequence'/Game/AnimStarterPack/Hit_React_2.Hit_React_2'"));
	if (AnimSeq)
	{
		//第一种方法 ，播放了无法还原动作
		//if (AAnimationProjectCharacter* Character = Cast<AAnimationProjectCharacter>(GetPawn()))
		//{
		//	if (USkeletalMeshComponent *Mesh = Character->GetMesh())
		//	{
		//		Mesh->PlayAnimation(AnimSeq, false);
		//	}
		//}
		//通过Montage的Slot可以完成自动切换动作
		float AnimLength = 0.f;
		if (USkeletalMeshComponent *Mesh = Cast<AAnimationProjectCharacter>(GetPawn())->GetMesh())
		{
			if (UAnimInstance *AnimInst = Mesh->GetAnimInstance())
			{
				if (UAnimMontage* Mtg = AnimInst->PlaySlotAnimationAsDynamicMontage(AnimSeq, TEXT("DefaultSlot"), 0.25f, 0.25f, 1.f, 1))
				{
					AnimLength = Mtg->GetPlayLength();
				}
			}
		}
	}


}





