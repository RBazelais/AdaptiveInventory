// InventorySlotWidget.h
#pragma once

#include "CoreMinimal.h"

#include "UI/InventoryWidgetBase.h"

#include "UI/InventoryStyleTypes.h"

#include "Core/InventoryItemData.h"

#include "InventorySlotWidget.generated.h"

class UImage;
class UTextBlock;
class UBorder;
class UInventoryStyleData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, UInventorySlotWidget * , Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotHovered, UInventorySlotWidget * , Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUnhovered, UInventorySlotWidget * , Slot);

UCLASS(Blueprintable)
class ADAPTIVEINVENTORY_API UInventorySlotWidget: public UInventoryWidgetBase {
    GENERATED_BODY()

    public:
        UInventorySlotWidget(const FObjectInitializer & ObjectInitializer);

    virtual void NativeConstruct() override;
    virtual void NativeOnMouseEnter(const FGeometry & InGeometry,
        const FPointerEvent & InMouseEvent) override;
    virtual void NativeOnMouseLeave(const FPointerEvent & InMouseEvent) override;
    virtual FReply NativeOnMouseButtonDown(const FGeometry & InGeometry,
        const FPointerEvent & InMouseEvent) override;

    virtual void RefreshWidget_Implementation() override;

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    void SetItem(UInventoryItemData * NewItem);

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    UInventoryItemData * GetItem() const {
        return CurrentItem;
    }

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    bool HasItem() const {
        return CurrentItem != nullptr;
    }

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    void ClearSlot();

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    void SetSelected(bool bNewSelected);

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    bool IsSlotSelected() const {
        return bIsSelected;
    }

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    void SetSlotIndex(int32 NewIndex) {
        SlotIndex = NewIndex;
    }

    UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
    int32 GetSlotIndex() const {
        return SlotIndex;
    }

    UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
    FOnSlotClicked OnSlotClicked;

    UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
    FOnSlotHovered OnSlotHovered;

    UPROPERTY(BlueprintAssignable, Category = "Inventory Slot|Events")
    FOnSlotUnhovered OnSlotUnhovered;

    /** Get the color for a given rarity */
    UFUNCTION(BlueprintCallable, Category = "Inventory Slot|Visuals")
    FLinearColor GetRarityColor(EItemRarity Rarity) const;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot|Style")
    TObjectPtr < UInventoryStyleData > StyleData;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot|Style")
    FInventorySlotStyle SlotStyle;

    const FInventorySlotStyle & GetSlotStyle() const;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
    TObjectPtr < UBorder > SlotBorder;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
    TObjectPtr < UImage > ItemIcon;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
    TObjectPtr < UTextBlock > StackCountText;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
    TObjectPtr < UImage > RarityBar;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget, OptionalWidget = true), Category = "Inventory Slot|Widgets")
    TObjectPtr < UImage > SelectionHighlight;

    UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
    void UpdateVisuals();
    virtual void UpdateVisuals_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
    void UpdateIcon();
    virtual void UpdateIcon_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
    void UpdateStackCount();
    virtual void UpdateStackCount_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
    void UpdateRarityDisplay();
    virtual void UpdateRarityDisplay_Implementation();

    UFUNCTION(BlueprintNativeEvent, Category = "Inventory Slot|Visuals")
    void UpdateSelectionState();
    virtual void UpdateSelectionState_Implementation();

    private:
        UPROPERTY()
    TObjectPtr < UInventoryItemData > CurrentItem;

    bool bIsSelected = false;
    bool bIsHovered = false;
    int32 SlotIndex = -1;

    struct FSlotDisplayData {
        bool bHasItem = false;
        bool bIsStackable = false;
        bool bIsFullStack = false;
        int32 StackCount = 0;
        int32 MaxStack = 1;
        EItemRarity Rarity = EItemRarity::Common;
        UTexture2D * Icon = nullptr;
    };

    FSlotDisplayData GatherDisplayData() const;
    FLinearColor CalculateBorderColor(const FSlotDisplayData & Data) const;
};