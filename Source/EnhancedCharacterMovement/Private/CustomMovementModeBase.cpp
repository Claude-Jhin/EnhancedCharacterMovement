// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomMovementModeBase.h"

#include "EnhancedCharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

UWorld* UCustomMovementModeBase::GetWorld() const
{
	if (!HasAnyFlags(RF_ClassDefaultObject) && ensureMsgf(
			GetOuter(), TEXT("MovementModeBase: %s has a null OuterPrivate in UMovementModeBase::GetWorld()"),
			*GetFullName())
		&& !GetOuter()->HasAnyFlags(RF_BeginDestroyed) && !GetOuter()->IsUnreachable())
	{
		if (ULevel* Level = GetTypedOuter<ULevel>())
		{
			return Level->OwningWorld;
		}
	}
	return nullptr;
}

UEnhancedCharacterMovementComponent* UCustomMovementModeBase::GetEnhancedCharacterMovement_Implementation()
{
	return Cast<UEnhancedCharacterMovementComponent>(GetOuter());
}

ACharacter* UCustomMovementModeBase::GetCharacterOwner() const
{
	return IsValid(OwnerMovementComponent) ? OwnerMovementComponent->GetCharacterOwner() : nullptr;
}

void UCustomMovementModeBase::InitializeInternal()
{
	OwnerMovementComponent = GetEnhancedCharacterMovement();
	if (IsValid(OwnerMovementComponent))
	{
		Initialize();
		bInitialized = true;
	}
}


void UCustomMovementModeBase::BeginModeInternal()
{
	if (!IsValid(OwnerMovementComponent) || !bInitialized)
	{
		return;
	}
	OnBeginMode();
}

void UCustomMovementModeBase::EndModeInternal()
{
	if (!IsValid(OwnerMovementComponent) || !bInitialized)
	{
		return;
	}
	OnEndMode();
}

bool UCustomMovementModeBase::CanEnterModeInternal()
{
	return IsValid(OwnerMovementComponent) && CanEnterMode();
}

void UCustomMovementModeBase::UpdateInternal(float DeltaTime)
{
	if (OwnerMovementComponent && bInitialized)
	{
		Update(DeltaTime);
	}
}

void UCustomMovementModeBase::Initialize_Implementation()
{
}

bool UCustomMovementModeBase::CanEnterMode_Implementation()
{
	return true;
}

void UCustomMovementModeBase::OnBeginMode_Implementation()
{
}

void UCustomMovementModeBase::OnEndMode_Implementation()
{
}

void UCustomMovementModeBase::SetMovementMode(EMovementMode NewMovementMode)
{
	if (IsValid(OwnerMovementComponent))
	{
		OwnerMovementComponent->SetMovementMode(NewMovementMode);
	}
}

void UCustomMovementModeBase::SetCustomMovementMode(TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass)
{
	if (IsValid(OwnerMovementComponent))
	{
		OwnerMovementComponent->SetCustomMovementMode(NewCustomMovementModeClass);
	}
}

float UCustomMovementModeBase::GetGravityZ() const
{
	return IsValid(OwnerMovementComponent)
		       ? OwnerMovementComponent->GetGravityZ()
		       : 0.0f;
}

bool UCustomMovementModeBase::MoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation, bool bSweep)
{
	return (IsValid(OwnerMovementComponent) && bInitialized)
		       ? OwnerMovementComponent->MoveUpdatedComponent(Delta, NewRotation, bSweep)
		       : false;
}

void UCustomMovementModeBase::SetVelocity(FVector NewVelocity)
{
	if (IsValid(OwnerMovementComponent))
	{
		OwnerMovementComponent->Velocity = NewVelocity;
	}
}

FVector UCustomMovementModeBase::GetVelocity() const
{
	if (IsValid(OwnerMovementComponent))
	{
		return OwnerMovementComponent->Velocity;
	}

	return FVector::ZeroVector;
}

void UCustomMovementModeBase::Update_Implementation(float DeltaTime)
{
	if (!IsValid(OwnerMovementComponent))
	{
		return;
	}
}
