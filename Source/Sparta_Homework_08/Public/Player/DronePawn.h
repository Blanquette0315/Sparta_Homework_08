#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DronePawn.generated.h"

class UPhysicsHandleComponent;
class UArrowComponent;
struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;
class UCapsuleComponent;

UCLASS()
class SPARTA_HOMEWORK_08_API ADronePawn : public APawn
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ForwardArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* RightArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* UpArrowComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* BackwardArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* LeftArrowComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Component", Meta = (AllowPrivateAccess = "true"))
	UArrowComponent* DownArrowComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	FVector CurrentVelocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentRoll;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="DroneMoveMent|Property", Meta = (AllowPrivateAccess = "true"))
	float CurrentPitch;

	FVector MoveInput;
	bool bMove;
	bool bTiltReverse;

	float TargetSpeed;
	float AccelInterp;
	float TargetRoll ;
	float TargetPitch;
	float Gravity;
	float VerticalVelocity; // 현재 Z축 속도
	bool bIsGround;

	bool bDroneOff;
	bool bIsGrabbing;

	float BladeRotationSpeed;
	float BaseBladeRotationSpeed;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UStaticMeshComponent* WingStaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UStaticMeshComponent* Wing2StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Drone|Component")
	UPhysicsHandleComponent* PhysicsHandleComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera|Component")
	UCameraComponent* CameraComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	bool Is_UseArrowDirection;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float Acceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float Deceleration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float RotationInterpSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float MaxTiltAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float TiltSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float SpeedFactor; // 속도 조절 인자 (값이 클수록 더 빠르게 변화)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneMoveMent|Property")
	float AccelerationCurveExponent;

	// 타이머 핸들과 속도 감소 값을 매핑
	TMap<int32, TPair<FTimerHandle, float>> SpeedEffectMap;
	int32 ID = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	float GrabLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	float HoldDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DroneGrab|Property")
	FVector GrabDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth;
	
public:
	ADronePawn();

protected:
	virtual void BeginPlay() override;

	void RestoreSpeed(int32 Key);
	
public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);

	float GetHealthPercentage() {return CurrentHealth/MaxHealth;}
	
	void OnDeath();
	void UpdateOverheadHP();
	
	void SetEnhancedInput();

	void CheckGround();

	float GetSpeedPercentage();
	const float GetCurrentSpeed() {return CurrentSpeed;}
	const float GetmaxSpeed() {return MaxSpeed;}

	void DecreaseSpeed(const float& Amount, const float& MaintainTime = 0.0f);
private:
	void InputUseArrowDirection();
	void InputUseControllerDirection();
	
protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void DroneOff(const FInputActionValue& value);
	UFUNCTION()
	void ReturnPlayer(const FInputActionValue& value);
};
