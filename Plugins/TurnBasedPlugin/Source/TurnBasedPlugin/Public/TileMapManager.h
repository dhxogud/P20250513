// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileMapManager.generated.h"

UCLASS()
class TURNBASEDPLUGIN_API ATileMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileMapManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    // Ÿ�� �� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
    void GenerateTileMap(int32 MapWidth, int32 MapHeight);

	// Ÿ�� �ʱ�ȭ �� ���� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	void ClearTiles();

	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	ATileBase* GetTileAtIndex(int32 Index);

	// Ÿ���� ��ġ�� ã�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Tile Map")
	ATileBase* GetTileAt(FVector Location);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile Map")
	float TileSize;  // Ÿ�� ũ��

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile Map")
	int32 m_MapWidth;  // ���� �ʺ�

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Tile Map")
	int32 m_MapHeight; // ���� ����

	// Ÿ���� ������ �� ����� Ŭ���� (�������Ʈ���� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Map")
	TSubclassOf<ATileBase> TileClass;  // ATile�� �ڽ� Ŭ������ ���� ����

    // Ÿ�� �迭 (��)
    // UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile Map") <-- ������ �迭�� �������Ʈ ���� �Ұ���
    // TArray<TArray<ATileBase*>> TileMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile Map")
	TArray<ATileBase*> TileMap; // 1���� �迭�� ����

//---------------------------------------------[ Begin A* ��� Ž�� ]------------------------------------------------//

public:
	// A* ��� Ž�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "A Star")
	TArray<ATileBase*> FindPath(FVector StartLocation, FVector EndLocation);

	// Ÿ���� �̿��� ���ϴ� �Լ�
	TArray<ATileBase*> GetNeighbors(ATileBase* Tile);

	// Ÿ���� F���� ����ϴ� �Լ�
	ATileBase* GetLowestCostTile(TArray<ATileBase*>& TileList);

//---------------------------------------------[ End A* ��� Ž�� ]------------------------------------------------//

};
