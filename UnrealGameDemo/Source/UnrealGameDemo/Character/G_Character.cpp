// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/G_Character.h"

// Sets default values
AG_Character::AG_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AG_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AG_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AG_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

