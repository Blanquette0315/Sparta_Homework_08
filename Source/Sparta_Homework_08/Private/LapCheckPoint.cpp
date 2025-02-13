#include "LapCheckPoint.h"
#include "Components/BoxComponent.h"
#include "GameBase/SpartaGameState.h"

ALapCheckPoint::ALapCheckPoint()
	: CheckPointNum(1)
	, bLastCheckPoint(false)
{
	PrimaryActorTick.bCanEverTick = false;

	BoxCompo = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxCompo);
	BoxCompo->SetCollisionProfileName("OverlapAllDynamic");

	// 이벤트 바인딩
	BoxCompo->OnComponentBeginOverlap.AddDynamic(this, &ALapCheckPoint::OnCheckPointOverlap);
	BoxCompo->OnComponentEndOverlap.AddDynamic(this, &ALapCheckPoint::OnCheckPointEndOverlap);
}

void ALapCheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALapCheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALapCheckPoint::OnCheckPointOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		if (const UWorld* World = GetWorld())
		{
			if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
			{
				// 역주행 감지 (이전 Lap이 본인 이전 Lap인가?)
				if (CheckPointNum == GameState->GetLastCheckPointNum() + 1)
				{
					// 마지막 Lap일 때, 랩 카운트를 올려주는 함수를 호출해주어야함.
					if(bLastCheckPoint)
					{
						GameState->IncreaseLap();
						GameState->SetChangeLap(false);
						GameState->SetLasteCheckPointNum(0);
					}
					else
					{
						GameState->SetChangeLap(true);
						GameState->SetLasteCheckPointNum(CheckPointNum);
					}
				}
			}
		}
	}
}

void ALapCheckPoint::OnCheckPointEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}