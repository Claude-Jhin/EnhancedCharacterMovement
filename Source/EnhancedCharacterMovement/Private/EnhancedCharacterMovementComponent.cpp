// Fill out your copyright notice in the Description page of Project Settings.


#include "EnhancedCharacterMovementComponent.h"
#include "CustomMovementModeBase.h"

const FName FallingTagName(TEXT("CharacterState.MovementState.Falling"));
const FName CustomTagName(TEXT("CharacterState.MovementState.Custom"));
const FName WalkingTagName(TEXT("CharacterState.MovementState.Walking"));

UEnhancedCharacterMovementComponent::UEnhancedCharacterMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UEnhancedCharacterMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UEnhancedCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);

	if (IsInCustomMovementMode(WalkingTagName))
	{
		CurrentCustomMovementMode->UpdateInternal(deltaTime);
	}
}

void UEnhancedCharacterMovementComponent::PhysFalling(float deltaTime, int32 Iterations)
{
	Super::PhysFalling(deltaTime, Iterations);

	if (IsInCustomMovementMode(FallingTagName))
	{
		CurrentCustomMovementMode->UpdateInternal(deltaTime);
	}
}

void UEnhancedCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);

	if (IsInCustomMovementMode(CustomTagName))
	{
		CurrentCustomMovementMode->UpdateInternal(deltaTime);
	}
}

void UEnhancedCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode,
                                                                uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);

	if (PreviousMovementMode == MOVE_Custom)
	{
		// If still in custom mode, which means no new mode is pushed, then release it 
		if (IsValid(CurrentCustomMovementMode) && IsInCustomMovementMode(CustomTagName))
		{
			CurrentCustomMovementMode->EndModeInternal();
			CurrentCustomMovementMode = nullptr;
		}
	}
	else if (PreviousMovementMode == MOVE_Falling)
	{
		// If still in falling mode, which means no new mode is pushed, then release it 
		if (IsValid(CurrentCustomMovementMode) && IsInCustomMovementMode(FallingTagName))
		{
			CurrentCustomMovementMode->EndModeInternal();
			CurrentCustomMovementMode = nullptr;
		}
	}
}

void UEnhancedCharacterMovementComponent::OnCustomMovementModeChanged(UCustomMovementModeBase* PrevMode)
{
	if (IsValid(PrevMode))
	{
		PrevMode->EndModeInternal();
	}

	if (IsValid(CurrentCustomMovementMode))
	{
		CurrentCustomMovementMode->BeginModeInternal();
	}
}

bool UEnhancedCharacterMovementComponent::IsInCustomMovementMode(FName MovementStateName)
{
	return CharacterOwner && IsValid(CurrentCustomMovementMode) && CurrentCustomMovementMode->GetMovementState().
		MatchesTag(FGameplayTag::RequestGameplayTag(MovementStateName));
}

void UEnhancedCharacterMovementComponent::SetCustomMovementMode(
	TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass)
{
	if (!CurrentCustomMovementMode)
	{
		auto TempMode = GetPooledMovementModeInstance(NewCustomMovementModeClass);
		// Check mode
		if (!TempMode->CanEnterModeInternal())
		{
			return;
		}
		CurrentCustomMovementMode = TempMode;
		OnCustomMovementModeChanged(nullptr);
	}
	else
	{
		if (NewCustomMovementModeClass == CurrentCustomMovementMode->GetClass())
		{
			return;
		}

		auto PrevMode = CurrentCustomMovementMode;
		auto TempMode = GetPooledMovementModeInstance(NewCustomMovementModeClass);

		if (!TempMode->CanEnterModeInternal())
		{
			return;
		}
		CurrentCustomMovementMode = TempMode;
		OnCustomMovementModeChanged(PrevMode);
	}

	// Update MovementMode
	if (IsInCustomMovementMode(CustomTagName))
	{
		SetMovementMode(MOVE_Custom);
	}
	else if (IsInCustomMovementMode(FallingTagName))
	{
		SetMovementMode(MOVE_Falling);
	}
	
	Server_SetCustomMovementMode(NewCustomMovementModeClass);
}

void UEnhancedCharacterMovementComponent::NetMulticast_SetCustomMovementMode_Implementation(
	TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass)
{
	SetCustomMovementMode(NewCustomMovementModeClass);
}

void UEnhancedCharacterMovementComponent::Server_SetCustomMovementMode_Implementation(
	TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass)
{
	NetMulticast_SetCustomMovementMode(NewCustomMovementModeClass);
}

UCustomMovementModeBase* UEnhancedCharacterMovementComponent::GetPooledMovementModeInstance(
	TSubclassOf<UCustomMovementModeBase> MovementModeClass)
{
	check(MovementModeClass);

	// First see if we already created one.
	for (UCustomMovementModeBase* Mode : MovementModePool)
	{
		if ((Mode != nullptr) && (Mode->GetClass() == MovementModeClass))
		{
			return Mode;
		}
	}

	// Not found, so we need to create it.
	UCustomMovementModeBase* NewMovementMode = NewObject<UCustomMovementModeBase>(
		this, MovementModeClass, NAME_None, RF_NoFlags);
	check(NewMovementMode);

	NewMovementMode->InitializeInternal();
	
	MovementModePool.Add(NewMovementMode);

	return NewMovementMode;
}
