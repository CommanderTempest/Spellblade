// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpellbladePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPELLBLADE_API ASpellbladePlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetPlayerEnabledState(bool bPlayerEnabled);
};
