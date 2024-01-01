// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
UENUM()
enum class ESpellementType
{
	Wind UMETA(DisplayName = "Wind"),
	Fire UMETA(DisplayName = "Fire"), 
	Water UMETA(DisplayName = "Water"), 
	Earth UMETA(DisplayName = "Earth"), 
	None UMETA(DisplayName="None")
};
