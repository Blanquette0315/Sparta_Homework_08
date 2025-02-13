#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrampolinePlatform.generated.h"

class USphereComponent;

UCLASS()
class SPARTA_HOMEWORK_08_API ATrampolinePlatform : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrampolinePlatform|Components")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TrampolinePlatform|Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrampolinePlatform|Properties")
	float Force;

	bool bCollisionOncec;
	
public:	
	ATrampolinePlatform();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	 virtual void OnOverlap(UPrimitiveComponent* OverlappedComp
	 		, AActor* OtherActor
	 		, UPrimitiveComponent* OtherComp
	 		, int32 OtherBodyIndex
	 		, bool bFromSweep,
	 		const FHitResult& SweepResult);
	UFUNCTION()
	 virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp
	 	, AActor* OtherActor
	 	, UPrimitiveComponent* OtherComp
	 	, int32 OtherBodyIndex);
	
	UFUNCTION(BlueprintCallable, Category="FallPlatform|Functions")
	virtual void OnPlatformHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
