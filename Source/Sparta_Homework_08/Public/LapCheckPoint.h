#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LapCheckPoint.generated.h"

class UBoxComponent;

UCLASS()
class SPARTA_HOMEWORK_08_API ALapCheckPoint : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="LapCheckPoint|Component")
	UBoxComponent* BoxCompo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LapCheckPoint|Property")
	int32 CheckPointNum;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LapCheckPoint|Property")
	bool bLastCheckPoint;
	
public:	
	ALapCheckPoint();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnCheckPointOverlap(UPrimitiveComponent* OverlappedComp
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep,
		const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnCheckPointEndOverlap(UPrimitiveComponent* OverlappedComp
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex) ;
	
public:	
	virtual void Tick(float DeltaTime) override;
};
