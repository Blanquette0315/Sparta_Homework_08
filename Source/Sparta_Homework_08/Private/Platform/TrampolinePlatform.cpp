#include "Platform/TrampolinePlatform.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Player/DronePawn.h"

ATrampolinePlatform::ATrampolinePlatform()
	: Force(0.f)
	, bCollisionOncec(false)
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComp);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComp->SetupAttachment(StaticMeshComp);
}

void ATrampolinePlatform::BeginPlay()
{
	Super::BeginPlay();

	//StaticMeshComp->OnComponentHit.AddDynamic(this, &ATrampolinePlatform::OnPlatformHit);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATrampolinePlatform::OnOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &ATrampolinePlatform::OnEndOverlap);
}

void ATrampolinePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrampolinePlatform::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void ATrampolinePlatform::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bCollisionOncec = false;
}

void ATrampolinePlatform::OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		UCapsuleComponent* CapsuleComp = OtherActor->GetComponentByClass<UCapsuleComponent>();
		if (CapsuleComp && CapsuleComp->IsSimulatingPhysics())
		{
			FVector ImpulseDirection = Hit.ImpactNormal * -1.f * Force; // 힘의 방향과 크기 설정
			CapsuleComp->AddImpulse(ImpulseDirection, NAME_None, true);
		}
		
		// Cast<APawn>(OtherActor)->addimplus
		// Cast<ASparta_Homework_06Character>(OtherActor)->LaunchCharacter(Hit.ImpactNormal * -1.f * Force, false, false);
	}
}
