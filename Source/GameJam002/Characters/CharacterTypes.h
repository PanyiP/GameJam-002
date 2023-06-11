#pragma once

#include "CoreMinimal.h"

UENUM()
enum class ECharacterDirection
{
	ECD_Left = 0,
	ECD_Right,
	ECD_MAX
};

UENUM()
enum class ECharacterState
{
	ECS_Idle = 0,
	ECS_Running,
	ECS_Attacking,
	ECD_MAX
};

static class CharacterLevelMap
{
public:
	static TMap<int32, float> CharLvlXpMap;
};
