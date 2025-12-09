// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/InventoryItemData.h"

UInventoryItemData::UInventoryItemData()
{
    // Default values
    ItemName = FText::FromString("New Item");
    ItemDescription = FText::FromString("Item description");
    ItemIcon = nullptr;
    ItemRarity = EItemRarity::Common;
    ItemCategory = EItemCategory::Material;
    
    CurrentStackSize = 1;
    MaxStackSize = 1;
    bIsStackable = false;
    
    MinDamage = 0.0f;
    MaxDamage = 0.0f;
    AttackSpeed = 1.0f;
    CurrentDurability = 100.0f;
    MaxDurability = 100.0f;
    Weight = 1.0f;
}

void UInventoryItemData::PostInitProperties()
{
    Super::PostInitProperties();
    
    // Generate unique GUID for this item instance
    if (!ItemGUID.IsValid())
    {
        ItemGUID = FGuid::NewGuid();
    }
}

bool UInventoryItemData::AddToStack(int32 Amount)
{
    if (!bIsStackable || Amount <= 0)
    {
        return false;
    }

    int32 NewStackSize = CurrentStackSize + Amount;
    if (NewStackSize > MaxStackSize)
    {
        CurrentStackSize = MaxStackSize;
        return false; // Stack is full, only added partial amount
    }

    CurrentStackSize = NewStackSize;
    return true;
}

bool UInventoryItemData::RemoveFromStack(int32 Amount)
{
    if (Amount <= 0 || CurrentStackSize < Amount)
    {
        return false; // Invalid amount or not enough in stack
    }

    CurrentStackSize -= Amount;
    return true;
}

bool UInventoryItemData::IsStackFull() const
{
    return CurrentStackSize >= MaxStackSize;
}

void UInventoryItemData::SetStackSize(int32 NewSize)
{
    // Clamp to valid range: at least 1, at most MaxStackSize
    CurrentStackSize = FMath::Clamp(NewSize, 1, MaxStackSize);
}