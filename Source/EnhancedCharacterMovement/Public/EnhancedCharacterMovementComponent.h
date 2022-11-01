// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedCharacterMovementComponent.generated.h"


/**
 * @brief Enhanced CharacterMovementComponent based on CharacterMovementComponent
 * Mainly use a simple state machine to switch custom movement mode
 */
UCLASS()
class ENHANCEDCHARACTERMOVEMENT_API UEnhancedCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	/**
	* Default UObject constructor.
	*/
	UEnhancedCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitializeComponent() override;

	virtual void PhysWalking(float deltaTime, int32 Iterations) override;

	virtual void PhysFalling(float deltaTime, int32 Iterations) override;

	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	UFUNCTION(BlueprintCallable, Category = "Custom Movement")
	void SetCustomMovementMode(TSubclassOf<class UCustomMovementModeBase> NewCustomMovementModeClass);

	UFUNCTION(Server, Reliable)
	void Server_SetCustomMovementMode(TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass);

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticast_SetCustomMovementMode(TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass);

	FORCEINLINE UCustomMovementModeBase* GetCurrentCustomMovementMode() const { return CurrentCustomMovementMode; }

#pragma region UTILS

#pragma endregion UTILS

protected:
	void OnCustomMovementModeChanged(UCustomMovementModeBase* PrevMode);

	bool IsInCustomMovementMode(FName MovementStateName);

	UCustomMovementModeBase* GetPooledMovementModeInstance(TSubclassOf<UCustomMovementModeBase> MovementModeClass);

	UPROPERTY(BlueprintReadOnly, Category = "Custom Movement")
	UCustomMovementModeBase* CurrentCustomMovementMode;

	UPROPERTY(BlueprintReadOnly)
	TArray<UCustomMovementModeBase*> MovementModePool;
	
};
