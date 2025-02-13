#include "Player/DronePawn.h"

#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Player/SpartaPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameBase/SpartaGameState.h"
#include "GameFramework/SpringArmComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

ADronePawn::ADronePawn()
	: CurrentVelocity(FVector::ZeroVector)
	, CurrentSpeed(0.0f)
	, CurrentRoll(0.0f)
	, CurrentPitch(0.0f)
	, MoveInput(FVector::ZeroVector)
	, bMove(false)
	, TargetSpeed(0.0f)
	, AccelInterp(0.0f)
	, TargetRoll (0.0f)
	, TargetPitch(0.0f)
	, Gravity(980.0f)
	, VerticalVelocity(0.0f)
	, bIsGround(false)
	, bDroneOff(false)
	, bIsGrabbing(false)
	, BaseBladeRotationSpeed(360.0f)
	, Is_UseArrowDirection(false)
	, MaxSpeed(1000.0f)
	, Acceleration(10.0f)
	, Deceleration(5.0f)
	, RotationSpeed(15.0f)
	, RotationInterpSpeed(10.0f)
	, MaxTiltAngle(60.0f)
	, TiltSpeed(3.0f)
	, SpeedFactor(1.0f)
	, AccelerationCurveExponent(0.3f)
	, GrabLength(100.0f)
	, HoldDistance(100.0f)
	, GrabDirection(FVector(0.0f, 0.0f, -1.0f))
	, MaxHealth(100.0f)
	, CurrentHealth(MaxHealth)
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->SetSimulatePhysics(false);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(CapsuleComp);
	StaticMeshComp->SetSimulatePhysics(false);

	WingStaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingMesh1"));
	WingStaticMeshComp->SetupAttachment(StaticMeshComp, FName("WingSocket1"));

	Wing2StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WingMesh2"));
	Wing2StaticMeshComp->SetupAttachment(StaticMeshComp, FName("WingSocket2"));

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandle"));

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = false;
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	
	ForwardArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrowComp->SetupAttachment(CapsuleComp);
	ForwardArrowComp->AddRelativeRotation(FRotator(MaxTiltAngle , 0.0f, 0.0f));
	ForwardArrowComp->SetHiddenInGame(true);
	
	RightArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("RightArrow"));;
	RightArrowComp->SetupAttachment(CapsuleComp);
	RightArrowComp->AddRelativeRotation(FRotator(MaxTiltAngle, 90.0f, 0.0f));
	RightArrowComp->SetArrowFColor(FColor::Green);
	RightArrowComp->SetHiddenInGame(true);
	
	UpArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("UpArrow"));
	UpArrowComp->SetupAttachment(CapsuleComp);
	UpArrowComp->AddRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	UpArrowComp->SetArrowFColor(FColor::Blue);
	UpArrowComp->SetHiddenInGame(true);

	BackwardArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("BackwardArrow"));
	BackwardArrowComp->SetupAttachment(CapsuleComp);
	BackwardArrowComp->AddRelativeRotation(FRotator(180.0f - MaxTiltAngle, 0.0f, 0.0f));
	BackwardArrowComp->SetHiddenInGame(true);
	
	LeftArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("LeftArrow"));;
	LeftArrowComp->SetupAttachment(CapsuleComp);
	LeftArrowComp->AddRelativeRotation(FRotator(MaxTiltAngle, 270.0f, 0.0f));
	LeftArrowComp->SetArrowFColor(FColor::Green);
	LeftArrowComp->SetHiddenInGame(true);
	
	DownArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("DownArrow"));
	DownArrowComp->SetupAttachment(CapsuleComp);
	DownArrowComp->AddRelativeRotation(FRotator(270.0f, 0.0f, 0.0f));
	DownArrowComp->SetArrowFColor(FColor::Blue);
	DownArrowComp->SetHiddenInGame(true);
}

void ADronePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckGround();
	// 이동
	FVector MoveDirection = ControlInputVector.GetSafeNormal();
	if (MoveDirection.IsNearlyZero())
	{
		MoveInput = FVector::ZeroVector;
		TargetSpeed = 0.0f;
		AccelInterp = Deceleration;
		TargetRoll = 0.0f;
		TargetPitch = 0.0f;
		MoveDirection = CurrentVelocity.GetSafeNormal();
	}
	else
	{
		TargetSpeed = MaxSpeed;
		AccelInterp = Acceleration;
		if (!FMath::IsNearlyZero(MoveInput.X))
		{
			if (Is_UseArrowDirection)
			{
				TargetRoll = MaxTiltAngle * MoveInput.X * -1.0f;
			}
			else
			{
				TargetRoll = MaxTiltAngle * -MoveInput.X * -1.0f;
			}
		}
		else
			TargetRoll = 0.0f;
		if (!FMath::IsNearlyZero(MoveInput.Y))
		{
			TargetPitch = MaxTiltAngle * -MoveInput.Y * -1.0f;
		}
		else
			TargetPitch = 0.0f;
	}

	// 선형 보간
	//CurrentSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, AccelInterp);
	// 사인 그래프를 이용한 보간
	float InterpAlpha = FMath::Clamp(CurrentSpeed / MaxSpeed, 0.1f, 1.0f);
	float AdjustedAlpha = FMath::Pow(InterpAlpha, AccelerationCurveExponent);
	float SmoothFactor = FMath::Sin(AdjustedAlpha * (PI / 2));  // 0~1 범위의 사인 곡선
	SmoothFactor *= SpeedFactor;
	CurrentSpeed = FMath::Lerp(CurrentSpeed, TargetSpeed, SmoothFactor * DeltaTime);
	
	if (FMath::IsNearlyZero(CurrentSpeed, 1.0f))
	{
		CurrentSpeed = 0.0f;
	}

	
	CurrentVelocity = MoveDirection * CurrentSpeed;

	// 이동 속도에 따른 날개 회전
	float TargetBladeRotationSpeed = BaseBladeRotationSpeed + (SpeedFactor * CurrentSpeed);
	BladeRotationSpeed = FMath::FInterpTo(BladeRotationSpeed, TargetBladeRotationSpeed, DeltaTime, 2.0f);
	FRotator CurrentRotation = WingStaticMeshComp->GetRelativeRotation();
	CurrentRotation.Yaw += BladeRotationSpeed * DeltaTime;
	WingStaticMeshComp->SetRelativeRotation(CurrentRotation);
	CurrentRotation = Wing2StaticMeshComp->GetRelativeRotation();
	CurrentRotation.Yaw += BladeRotationSpeed * DeltaTime;
	Wing2StaticMeshComp->SetRelativeRotation(CurrentRotation);
 	
	if (!bIsGround && bDroneOff)
	{
		if (ControlInputVector.IsNearlyZero())
		{
			VerticalVelocity += Gravity * DeltaTime;
			CurrentVelocity.Z -= VerticalVelocity;
		}
		else
		{
			VerticalVelocity = 0.0f;
		}
	}

	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;

	// 충돌 검사
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 자기 자신은 충돌 검사에서 제외
	FVector StartPoint = GetActorLocation();

	//DrawDebugSphere(GetWorld(), StartPoint, 15, 26, FColor::Red);
		
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,             // 충돌 결과 저장
		StartPoint,				 // 시작 위치
		NewLocation,             // 목표 위치
		FQuat::Identity,         // 회전 없음
		ECC_Pawn,                // 충돌 채널 (Pawn으로 설정)
		FCollisionShape::MakeSphere(50.0f), // 충돌 체크 모양 (구)
		Params
	);

	if (!bHit) 
	{
		SetActorLocation(NewLocation); // 충돌이 없으면 이동
	}
	else 
	{
		// 슬라이딩 처리: 벽의 법선(Normal)과 이동 방향을 사용하여 새로운 이동 벡터 계산
		FVector Direction = (GetActorLocation() - NewLocation) * -1;
		Direction = Direction.GetSafeNormal();
		FVector SlideDirection = FVector::VectorPlaneProject(Direction, HitResult.Normal);
		FVector SlideLocation = GetActorLocation() + (SlideDirection * CurrentVelocity.Length() * DeltaTime);

		// 슬라이딩 방향으로 재충돌 검사
		bool bSlideHit = GetWorld()->SweepSingleByChannel(
			HitResult,
			StartPoint,
			SlideLocation,
			FQuat::Identity,
			ECC_Pawn,
			FCollisionShape::MakeSphere(50.0f),
			Params
		);

		// 해결책 1: 벽에 완전히 정면으로 부딪힌 경우는 이동하지 않음
		if (FVector::DotProduct(Direction, HitResult.Normal) < -0.9f) {}
		else
		{
			if (!bSlideHit) 
			{
				// 슬라이딩 방향으로 이동 허용
				SetActorLocation(SlideLocation);
			}
			else 
			{
				UE_LOG(LogTemp, Warning, TEXT("벽에 완전히 막혀 이동 불가!"));
				FVector SmallOffset = HitResult.Normal * 2.0f;
				SetActorLocation(GetActorLocation() + SmallOffset);
			}
		}
	}

	bMove = false;
	Internal_ConsumeMovementInputVector();

	
	// 이동에 따른 회전
	FRotator TargetRotation = GetActorRotation();
	FRotator PrevSprintArmRot = SpringArmComp->GetComponentRotation();
	if (!Is_UseArrowDirection)
		TargetRotation = StaticMeshComp->GetRelativeRotation();
		
	// 선형 보간
	//CurrentRoll = FMath::FInterpTo(CurrentRoll, TargetRoll, DeltaTime, TiltSpeed);
	//CurrentPitch = FMath::FInterpTo(CurrentPitch, TargetPitch, DeltaTime, TiltSpeed);

	// 사인 그래프 보간
	CurrentRoll = FMath::Lerp(CurrentRoll, TargetRoll, SmoothFactor * DeltaTime);
	CurrentPitch = FMath::Lerp(CurrentPitch, TargetPitch, SmoothFactor * DeltaTime);
	
	if (Is_UseArrowDirection)
	{
		TargetRotation.Roll = CurrentPitch;
		TargetRotation.Pitch = CurrentRoll;
		SetActorRotation(TargetRotation);
	}
	else
	{
		TargetRotation.Pitch = CurrentPitch;
    	TargetRotation.Roll = CurrentRoll;
		StaticMeshComp->SetRelativeRotation(TargetRotation);
	}

	FRotator CalRot = PrevSprintArmRot - SpringArmComp->GetComponentRotation();
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	SpringArmRotation.Pitch += CalRot.Pitch;
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch, -89.0f, 89.0f);
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
}

float ADronePawn::GetHealth() const
{
	return CurrentHealth;
}

void ADronePawn::AddHealth(float Amount)
{
	CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

float ADronePawn::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.f, MaxHealth);
	UpdateOverheadHP();
	
	if (CurrentHealth <= 0.0f)
	{
		OnDeath();
	}
	
	return ActualDamage;
}

// OnDeath 구현해야함.
void ADronePawn::OnDeath()
{
	// ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() :nullptr;
	// if (SpartaGameState)
	// {
	// 	SpartaGameState->OnGameOver();
	// }
}

// UI 설정해주어야 함.
void ADronePawn::UpdateOverheadHP()
{
	if (UWorld* World = GetWorld())
	{
		if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
		{
			GameState->HPBarHUDUpdate();
		}
	}
}

void ADronePawn::SetEnhancedInput()
{
	InputComponent->ClearActionBindings();
	SetupPlayerInputComponent(InputComponent);
}

void ADronePawn::CheckGround()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0, 0, 50);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	if (GetWorld()->SweepSingleByChannel(HitResult,Start,End,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeSphere(45.0f),	Params))
	{
		bIsGround = true;
		VerticalVelocity = 0.f;
	} 
	else
	{		
		bIsGround = false;
	}
}

float ADronePawn::GetSpeedPercentage()
{
	return FMath::Clamp(CurrentSpeed / MaxSpeed, 0.0f, 1.0f);
}

void ADronePawn::RestoreSpeed(int32 Key)
{
	if (SpeedEffectMap.Contains(Key))
	{
		float ExpiredSpeedReduction = SpeedEffectMap[Key].Value;
		
		MaxSpeed += ExpiredSpeedReduction;
		
		GetWorld()->GetTimerManager().ClearTimer(SpeedEffectMap[Key].Key);
		SpeedEffectMap.Remove(Key);
	}
}

void ADronePawn::DecreaseSpeed(const float& Amount, const float& MaintainTime)
{
	if (MaintainTime != 0.0f)
	{
		ID++;
		FTimerHandle NewTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
		   NewTimerHandle, 
		   FTimerDelegate::CreateUObject(this, &ADronePawn::RestoreSpeed, ID), 
		   MaintainTime, 
		   false
	   );
		SpeedEffectMap.Add(ID,{ NewTimerHandle, Amount});
		
		MaxSpeed -= Amount;
	}
}

void ADronePawn::InputUseArrowDirection()
{
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		if (MoveInput.X > 0.0f)
			AddMovementInput(ForwardArrowComp->GetForwardVector(), MoveInput.X);
		else
			AddMovementInput(BackwardArrowComp->GetForwardVector(), MoveInput.X * -1.0f);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		if (MoveInput.Y > 0.0f)
			AddMovementInput(RightArrowComp->GetForwardVector(), MoveInput.Y);
		else
			AddMovementInput(LeftArrowComp->GetForwardVector(), MoveInput.Y * -1.0f);
	}
	if (!FMath::IsNearlyZero(MoveInput.Z))
	{
		if (MoveInput.Z > 0.0f)
			AddMovementInput(UpArrowComp->GetForwardVector(), MoveInput.Z);
		else
			AddMovementInput(DownArrowComp->GetForwardVector(), MoveInput.Z * -1.0f);
	}
}

void ADronePawn::InputUseControllerDirection()
{
	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
	if (!FMath::IsNearlyZero(MoveInput.Z))
	{
		AddMovementInput(GetActorUpVector(), MoveInput.Z);
	}	
}

void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput
		= Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ASpartaPlayerController* PlayerController
			= Cast<ASpartaPlayerController>(GetController()))
		{
			if (PlayerController->DronMoveAction)
			{
				EnhancedInput->BindAction(PlayerController->DronMoveAction
					, ETriggerEvent::Triggered
					,this
					, &ADronePawn::Move);
			}

			if (PlayerController->DronLookAction)
			{
				EnhancedInput->BindAction(PlayerController->DronLookAction
					, ETriggerEvent::Triggered
					,this
					, &ADronePawn::Look);
			}

			if (PlayerController->DroneOffAction)
			{
				EnhancedInput->BindAction(PlayerController->DroneOffAction
					, ETriggerEvent::Triggered
					,this
					, &ADronePawn::DroneOff);
			}

			if (PlayerController->DroneReturnPlayerAction)
			{
				EnhancedInput->BindAction(PlayerController->DroneReturnPlayerAction
					, ETriggerEvent::Triggered
					,this
					, &ADronePawn::ReturnPlayer);
			}
		}
	}
}

  void ADronePawn::Move(const FInputActionValue& value)
  {
  	if (!Controller) return;
	if (bDroneOff) return;
 
	bMove = true;
  	MoveInput = value.Get<FVector>();

	if (Is_UseArrowDirection)
	{
		InputUseArrowDirection();
	}
	else
	{
		InputUseControllerDirection();
	}
  }

 void ADronePawn::Look(const FInputActionValue& value)
 {
	if (!Controller) return;
	
	FVector LookInput = value.Get<FVector>();

	// 액터 회전 (Yaw 적용)
	FRotator ActorRotation = GetActorRotation();
	ActorRotation.Yaw += LookInput.X;
	SetActorRotation(ActorRotation);
	
	// SpringArm 회전
	FRotator SpringArmRotation = SpringArmComp->GetRelativeRotation();
	SpringArmRotation.Pitch += LookInput.Y;
	
	// Pitch 각도 제한 (-89 ~ 89도)
	SpringArmRotation.Pitch = FMath::Clamp(SpringArmRotation.Pitch, -89.0f, 89.0f);
	
	// 최종 회전 적용
	SpringArmComp->SetRelativeRotation(SpringArmRotation);
 }

void ADronePawn::ReturnPlayer(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		Cast<ASpartaPlayerController>(GetController())->ChangeMappingContext(0);
		Cast<ASpartaPlayerController>(GetController())->ChangePossess(Cast<ASpartaPlayerController>(GetController())->GetPlayerPawn());

		Destroy();
	}
}

void ADronePawn::DroneOff(const FInputActionValue& value)
{
	if (value.Get<bool>())
	{
		bDroneOff = !bDroneOff;
		if (bDroneOff)
		{
			BaseBladeRotationSpeed = 0.0f;
		}
		else
		{
			BaseBladeRotationSpeed = 360.0f;
		}
		VerticalVelocity = 0.f;
	}
}
