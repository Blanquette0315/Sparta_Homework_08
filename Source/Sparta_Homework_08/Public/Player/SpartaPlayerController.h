#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTA_HOMEWORK_08_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	APawn* PlayerPawn;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SpawnDroneAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* ChangePlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* GrabAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|CharacterPawn")
	UInputAction* SelectRotAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputMappingContext* DronInputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DronMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DronLookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneOffAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneReturnPlayerAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|DronePawn")
	UInputAction* DroneGrabAction;


	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(editAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	UFUNCTION(BlueprintCallable, Category= "HUD")
	void ShowGameHUD();
	//UFUNCTION(BlueprintCallable, Category= "Menu")
	//void ShowMainMenu(bool bIsRestart);
	//UFUNCTION(BlueprintCallable, Category= "Menu")
	//void StartGame();

protected:
	virtual void BeginPlay() override;

public:
	ASpartaPlayerController();

	void SetPlayerPawn(APawn* PlayerCharacterPawn) { PlayerPawn = PlayerCharacterPawn; }
	APawn* GetPlayerPawn() const { return PlayerPawn; }
	void ChangePossess(APawn* NewPawn);
	void ChangeMappingContext(int Type);
};
