// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "ChefCharacter.generated.h"

UCLASS()
class SHOOTYSHOT_API AChefCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChefCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

    UPROPERTY(EditAnywhere, Category = "Chef Interaction")
    UPhysicsHandleComponent* PhysicsHandle;

    UPROPERTY(EditAnywhere)
        float castDistance;

    /** First person camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* FirstPersonCameraComponent;

    UFUNCTION(BlueprintCallable, Category = "Chef Testing")
        virtual FString DoTrace();

    /** Fires a projectile. */
	UFUNCTION(BlueprintCallable, Category = "Chef|Interaction")
    void HandleGrabbingStuff(FRotator rotation, bool & succeeded, bool & grabbing,/* FString& linetrace,*/ FVector& ImpulseToAdd, UPrimitiveComponent*& ObjectToFling);

	UFUNCTION(BlueprintCallable, Category = "Chef|Interaction")
	void AddImpulse(UPrimitiveComponent* PhysicsObject, FVector ImpulseToAdd);


public:

    /** Returns FirstPersonCameraComponent subobject **/
    FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:
    bool PhysicsHandleActive;
    bool Held;
    float OtherItemLocation;
    float PickupDistance;
    FVector HandleLocation;
    FRotator OtherRotation;
    UPrimitiveComponent* PhysicsObject;
    EObjectTypeQuery PhysicsObjectTypes;

};
