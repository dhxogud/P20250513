// Fill out your copyright notice in the Description page of Project Settings.


#include "TileMapManager.h"
#include "Engine/World.h"
#include "TileBase.h"

// Sets default values
ATileMapManager::ATileMapManager() : TileSize(100.f) // �⺻ Ÿ�� ũ�� ����
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATileMapManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATileMapManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Ÿ�� �� ���� �Լ�
void ATileMapManager::GenerateTileMap(int32 MapWidth, int32 MapHeight)
{
    if (!TileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("TileClass is not set in ATileMapManager!"));
        return;
    }

    if (TileMap.IsEmpty() == false)
    {
		ClearTiles();  // ���� Ÿ���� �����ϸ� ����
    }

	m_MapWidth = MapWidth;  // ���� �ʺ� ����
	m_MapHeight = MapHeight; // ���� ���� ����

    // Ÿ�� �迭 ũ�� ����
    TileMap.SetNum(MapWidth * MapHeight);  // 1D �迭�� ũ�� ����

    for (int32 Y = 0; Y < MapHeight; ++Y)
    {
        for (int32 X = 0; X < MapWidth; ++X)
        {
            // Ÿ�� ����
            ATileBase * NewTile = GetWorld()->SpawnActor<ATileBase>(TileClass, FVector::ZeroVector, FRotator::ZeroRotator);

            // Ÿ���� ũ�⸦ ����
            TileSize = NewTile->GetTileSize();
            FVector Location(X * TileSize, Y * TileSize, 0.f);
			Location += FVector(TileSize / 2.f, TileSize / 2.f, 0.f);// 0,0 Ÿ���� TileMapManager�� �߽ɿ� ���߱� ���� ��ġ ����

            // FSearchTileData �ʱ�ȭ
            FSearchTileData SearchTileData;
            SearchTileData.X = X;
            SearchTileData.Y = Y;
            SearchTileData.GCost = 0;
            SearchTileData.HCost = 0;
            SearchTileData.FCost = 0;
            SearchTileData.Parent = nullptr;

            NewTile->SearchTileData = SearchTileData;
			NewTile->TileMapManager = this;  // Ÿ�� �Ŵ��� ����
            NewTile->bIsWalkable = true; // �⺻�� ����

            FString TileName = FString::Printf(TEXT("Tile%d%d"), X, Y);  // Ÿ�� �̸� ����
            NewTile->SetActorLabel(TileName);
			NewTile->SetActorLocation(Location + GetActorLocation());  // Ÿ�� ��ġ ����

            // 1D �迭�� ����
            TileMap[Y * MapWidth + X] = NewTile;  // 1D �迭�� ����
        }
    }
}

// Ÿ�� �ʱ�ȭ �� ���� �Լ�
void ATileMapManager::ClearTiles()
{
    // Ÿ�� �迭�� ��ȸ�ϸ� Ÿ���� ����
    for (int32 i = 0; i < TileMap.Num(); ++i)
    {
        ATileBase* Tile = TileMap[i];
        if (Tile)
        {
            // Ÿ�� �ı�
            Tile->Destroy();
        }
    }

    // Ÿ�� �迭 �ʱ�ȭ
    TileMap.Empty();
}

ATileBase* ATileMapManager::GetTileAtIndex(int32 Index)
{
    // ��ȿ�� ���� ���� �ִ��� Ȯ��
    if (Index >= 0 && Index < (m_MapWidth * m_MapHeight))
    {
        // ��ȿ�� �ε����� ���
        return TileMap[Index];
    }

    return nullptr;
}

ATileBase* ATileMapManager::GetTileAt(FVector Location)
{
    // Ÿ�� ũ�⸦ �������� X, Y ��ǥ�� ���
    int32 X = FMath::FloorToInt(Location.X / TileSize);
    int32 Y = FMath::FloorToInt(Location.Y / TileSize);

    // ��ȿ�� ���� ���� �ִ��� Ȯ��
    int32 Index = Y * m_MapWidth + X; // 1D �ε����� ��ȯ

    // ��ȿ�� ���� ���� �ִ��� Ȯ��
    if (Index >= 0 && Index < (m_MapWidth * m_MapHeight))
    {
        // ��ȿ�� �ε����� ���
        return TileMap[Index];
    }

    UE_LOG(LogTemp, Warning, TEXT("GetTileAt: Invalid Location! X: %d, Y: %d"), X, Y);  // ���� �޽��� ���
    return nullptr; // ��ȿ���� ���� ��� null ��ȯ
}

TArray<ATileBase*> ATileMapManager::FindPath(FVector StartLocation, FVector EndLocation)
{
    TArray<ATileBase*> OpenList;
    TArray<ATileBase*> ClosedList;

    // �������� ��ǥ�� Ÿ�� ã��
    ATileBase* StartTile = GetTileAt(StartLocation);
    ATileBase* GoalTile = GetTileAt(EndLocation);

    if (!StartTile || !GoalTile)
    {
        UE_LOG(LogTemp, Warning, TEXT("StartTile or GoalTile is nullptr!"));
        return TArray<ATileBase*>(); // �������̳� ��ǥ���� ��ȿ���� ������ �� ��� ��ȯ
    }

    OpenList.Add(StartTile);

    while (OpenList.Num() > 0)
    {
        // F���� ���� ���� Ÿ���� ����
        ATileBase* CurrentTile = GetLowestCostTile(OpenList);

        // ��ǥ Ÿ�Ͽ� ���������� ��� ���� ����
        if (CurrentTile == GoalTile)
        {
            UE_LOG(LogTemp, Warning, TEXT("GoalTile Arrived"));

            TArray<ATileBase*> Path;
            // ��� ����
            while (CurrentTile->SearchTileData.Parent != nullptr)
            {
                Path.Insert(CurrentTile, 0); // ��θ� �ڿ������� ����
                CurrentTile = CurrentTile->SearchTileData.Parent; // �θ� Ÿ�Ϸ� �̵�
            }

			// ��� �Ϸ� �� �θ� �ʱ�ȭ
            for (ATileBase* Tile : Path)
            {
                if (Tile)
                {
                    Tile->SearchTileData.Parent = nullptr;  // �θ� �ʱ�ȭ
                }
            }

            return Path; // ��� ��ȯ
        }

        OpenList.Remove(CurrentTile);
        ClosedList.Add(CurrentTile);

        // �̿� Ÿ�� Ž��
        TArray<ATileBase*> Neighbors = GetNeighbors(CurrentTile);
        for (ATileBase* Neighbor : Neighbors)
        {
            if (ClosedList.Contains(Neighbor))
                continue; // �̹� �˻��� Ÿ���� �ǳʶٱ�

			int32 TerrainCost = Neighbor->TerrainCost;// ���� ���
            int32 TentativeGCost = FVector::DistSquared(CurrentTile->GetActorLocation(), Neighbor->GetActorLocation()) + TerrainCost;

            // GCost�� �� �۰ų� OpenList�� ���� ���
            if (TentativeGCost < Neighbor->SearchTileData.GCost || !OpenList.Contains(Neighbor))
            {
                Neighbor->SearchTileData.Parent = CurrentTile;
                Neighbor->SearchTileData.GCost = TentativeGCost;
                Neighbor->SearchTileData.HCost = FVector::Dist(Neighbor->GetActorLocation(), GoalTile->GetActorLocation());
                Neighbor->SearchTileData.FCost = Neighbor->SearchTileData.GCost + Neighbor->SearchTileData.HCost;

                if (!OpenList.Contains(Neighbor))
                {
                    OpenList.Add(Neighbor);
                }
            }
        }
    }

    return TArray<ATileBase*>(); // ��θ� ã�� �� ������ �� �迭 ��ȯ
}

TArray<ATileBase*> ATileMapManager::GetNeighbors(ATileBase* Tile)
{
    TArray<ATileBase*> Neighbors;
    int32 X = Tile->SearchTileData.X;
    int32 Y = Tile->SearchTileData.Y;

    // 1D �迭���� �ε��� ���
    int32 Index = Y * m_MapWidth + X;

    // �����¿�, �밢�� �������� �̿� Ÿ���� Ž��
    TArray<FVector> Directions = {
        FVector(1, 0, 0),   // ������
        FVector(-1, 0, 0),  // ����
        FVector(0, 1, 0),   // ��
        FVector(0, -1, 0),  // �Ʒ�
        FVector(1, 1, 0),   // ���
        FVector(-1, -1, 0), // ����
        FVector(1, -1, 0),  // ����
        FVector(-1, 1, 0)   // �»�
    };

    for (const FVector& Direction : Directions)
    {
        int32 NewX = X + Direction.X;
        int32 NewY = Y + Direction.Y;

        // 1D �迭���� ��ȿ�� ���� ���� �ִ��� Ȯ��
        int32 NewIndex = NewY * m_MapWidth + NewX;

        if (NewIndex >= 0 && NewIndex < (m_MapWidth * m_MapHeight)) // ��ȿ�� �ε��� Ȯ��
        {
            ATileBase* Neighbor = TileMap[NewIndex];
            if (Neighbor && Neighbor->bIsWalkable) // �̵� ������ Ÿ�ϸ� �߰�
            {
                Neighbors.Add(Neighbor);
            }
        }
    }

    return Neighbors;
}

ATileBase* ATileMapManager::GetLowestCostTile(TArray<ATileBase*>& TileList)
{
    ATileBase* LowestCostTile = nullptr;

    for (ATileBase* Tile : TileList)
    {
        if (!LowestCostTile || Tile->SearchTileData.FCost < LowestCostTile->SearchTileData.FCost)
        {
            LowestCostTile = Tile;
        }
    }

    return LowestCostTile;
}
