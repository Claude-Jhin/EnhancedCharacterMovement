// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "CustomMovementModeBase.generated.h"

class UEnhancedCharacterMovementComponent;
/**
 * 
 */
UCLASS(Abstract, NotBlueprintable)
class ENHANCEDCHARACTERMOVEMENT_API UCustomMovementModeBase : public UObject
{
	GENERATED_BODY()
public:
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category ="Enhanced Movement")
	UEnhancedCharacterMovementComponent* GetEnhancedCharacterMovement();

	UFUNCTION(BlueprintCallable, Category = "Enhanced Movement")
	ACharacter* GetCharacterOwner() const;
	
	void InitializeInternal();
	
	void BeginModeInternal();

	void EndModeInternal();

	bool CanEnterModeInternal();

	void UpdateInternal(float DeltaTime);
	
	UFUNCTION(BlueprintNativeEvent)
	void Update(float DeltaTime);

	FORCEINLINE FGameplayTag GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintNativeEvent)
	void Initialize();
	
	UFUNCTION(BlueprintNativeEvent)
	bool CanEnterMode();

	virtual bool CanEnterMode_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void OnBeginMode();

	virtual void OnBeginMode_Implementation();

	virtual void Update_Implementation(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent)
	void OnEndMode();

	virtual void OnEndMode_Implementation();

protected:

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetMovementMode(EMovementMode NewMovementMode);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetCustomMovementMode(TSubclassOf<UCustomMovementModeBase> NewCustomMovementModeClass);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetGravityZ() const;
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool MoveUpdatedComponent(const FVector& Delta, const FRotator& NewRotation, bool bSweep);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void SetVelocity(FVector NewVelocity);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	FVector GetVelocity() const;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	class UEnhancedCharacterMovementComponent* OwnerMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", Meta = (Categories = "CharacterState.MovementState"))
	FGameplayTag MovementState;

private:
	uint8 bInitialized;
};
