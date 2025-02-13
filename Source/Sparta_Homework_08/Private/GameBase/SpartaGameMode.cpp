#include "GameBase/SpartaGameMode.h"
#include "GameBase/SpartaGameState.h"
#include "Player/SpartaCharacter.h"
#include "Player/SpartaPlayerController.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}