// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileBase.generated.h"

USTRUCT(BlueprintType)
struct FSearchTileData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 X;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 Y;

	// ���� Ÿ�Ͽ��� ���� Ÿ�ϱ����� �̵� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 GCost;

	// ���� Ÿ�Ͽ��� ��ǥ Ÿ�ϱ����� ������ �̵� ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 HCost;

	// GCost�� HCost�� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	int32 FCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	ATileBase* Parent;

	FSearchTileData() : X(0), Y(0), GCost(0), HCost(0), FCost(0), Parent(nullptr) {}
};


UCLASS()
class TURNBASEDPLUGIN_API ATileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Tile")
	float GetTileSize();

public:
	UPROPERTY(BlueprintReadOnly, Category = "Default", meta = (ExposeOnSpawn = "true"))
	class ATileMapManager* TileMapManager;

	// A* ��� Ž���� ���� ������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pathfinding")
	FSearchTileData SearchTileData;

	// �̵� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	bool bIsWalkable;

	// ���� ��� (��: ��ֹ�, �� ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pathfinding")
	uint8 TerrainCost;
};
