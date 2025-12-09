# Adaptive Inventory System

A performance-optimized inventory system built in Unreal Engine 5 with clean C++/Blueprint architecture.

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.7-blue?logo=unrealengine)
![C++](https://img.shields.io/badge/C++-17-00599C?logo=cplusplus)
![License](https://img.shields.io/badge/License-MIT-green)

---

## Overview

A production-ready inventory system following Epic's recommended patterns for UMG development. The project emphasizes:

- **Clean Architecture** — MVVM pattern with strict data/logic/UI separation
- **Performance** — Event-driven updates, zero tick overhead
- **Scalability** — Subsystem pattern persists across levels
- **Blueprint Integration** — Full designer workflow support

---

## Features

### Core Systems

- **Item Management** — Add, remove, stack, and query items
- **Auto-Stacking** — Intelligent overflow distribution across multiple stacks
- **Filtering** — By category, rarity, or partial name search
- **Event Broadcasting** — UI updates only when inventory changes

### Item Properties

| Property | Description |
|----------|-------------|
| Name / Description | Localized FText for display |
| Category | Weapon, Consumable, Material, Equipment, Quest |
| Rarity | Common, Uncommon, Rare, Epic, Legendary |
| Stack Size | Current and maximum stack capacity |
| Stats | Damage, speed, durability, weight |
| GUID | Unique identifier per item instance |

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                        PRESENTATION                         │
│                   (Blueprint Widgets - WIP)                 │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ Binds to Events
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                      SUBSYSTEM LOGIC                        │
│               UInventoryManagerSubsystem                    │
│                                                             │
│  • AddItem()           • GetItemsByCategory()               │
│  • RemoveItem()        • GetItemsByRarity()                 │
│  • TryStackItem()      • SearchItemsByName()                │
│                                                             │
│  Events: OnInventoryChanged, OnItemAdded, OnItemRemoved     │
└─────────────────────────────────────────────────────────────┘
                              │
                              │ Manages
                              ▼
┌─────────────────────────────────────────────────────────────┐
│                          DATA                               │
│                   UInventoryItemData                        │
│                                                             │
│  • Item properties (name, rarity, category, stats)          │
│  • Stack management (AddToStack, RemoveFromStack)           │
│  • GUID generation                                          │
└─────────────────────────────────────────────────────────────┘
```

---

## Quick Start

### 1. Access the Inventory Manager

```cpp
// C++
UInventoryManagerSubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventoryManagerSubsystem>();
```

```
// Blueprint
Get Inventory Manager → returns subsystem reference
```

### 2. Create and Add Items

```cpp
// C++
UInventoryItemData* Sword = NewObject<UInventoryItemData>(GetGameInstance());
Sword->ItemName = FText::FromString("Iron Sword");
Sword->ItemCategory = EItemCategory::Weapon;
Sword->ItemRarity = EItemRarity::Uncommon;
Inventory->AddItem(Sword);
```

```
// Blueprint
Create Weapon Item ("Iron Sword", MinDmg: 10, MaxDmg: 20)
    → Add Item (to Inventory Manager)
```

### 3. Listen for Changes

```cpp
// C++
Inventory->OnItemAdded.AddDynamic(this, &UMyWidget::HandleItemAdded);
Inventory->OnInventoryChanged.AddDynamic(this, &UMyWidget::RefreshDisplay);
```

```
// Blueprint
Bind Event to OnInventoryChanged → Refresh Inventory UI
```

---

## File Structure

```
Source/AdaptiveInventory/
├── Core/
│   ├── InventoryItemData.h/.cpp         # Item data class
│   ├── InventoryManagerSubsystem.h/.cpp # Inventory logic
│   └── InventoryBlueprintLibrary.h/.cpp # Blueprint helpers

Content/AdaptiveInventory/
├── UI/Widgets/        # UMG widgets (WIP)
├── Items/Icons/       # Item icons (WIP)
└── Testing/           # Test blueprints
```

---

## Blueprint Library Functions

### Item Creation

| Function | Description |
|----------|-------------|
| `CreateInventoryItem` | Full control over all properties |
| `CreateStackableMaterial` | Quick material (stack 99) |
| `CreateWeaponItem` | Weapon with damage stats |
| `CreateConsumableItem` | Stackable consumable (stack 20) |

### Utilities

| Function | Description |
|----------|-------------|
| `GetInventoryManager` | Get subsystem reference |
| `DebugPrintInventory` | Log all items to Output Log |
| `AddTestItemsToInventory` | Populate with random test data |

---

## Technical Decisions

### Why GameInstanceSubsystem?

- Persists across level loads without manual management
- Globally accessible without singleton boilerplate
- Built-in Blueprint exposure via `UFUNCTION`
- Automatic lifecycle (Initialize/Deinitialize)

### Why Event-Driven?

- Zero CPU cost when inventory is unchanged
- UI updates only on actual changes
- Scales to hundreds of items
- No per-frame polling or Tick overhead

### Why Separate Data Classes?

- Data lifetime independent of UI widgets
- Multiple widgets can display the same item
- Clean testability (logic without UI)
- Attempts to Follow Epic's UMG Best Practices

---

## Stacking System

The inventory supports intelligent auto-stacking with overflow handling:

```
Adding 80 Iron Ore (max stack: 99)
├── Existing stack: 50/99
├── After add: 99/99 (added 49)
└── Overflow: Creates new stack 31/99

Result: Two stacks totaling 130 items
```

Items match by **Name** + **Category**. The system distributes across all partial stacks before creating new slots.

---

## Roadmap

- [x] Core data structures
- [x] Inventory subsystem with CRUD operations
- [x] Auto-stacking with overflow
- [x] Blueprint function library
- [x] Event system for UI binding
- [ ] Widget base classes (C++)
- [ ] Inventory grid widget
- [ ] Item slot widget with rarity colors
- [ ] Drag and drop
- [ ] Detail panel
- [ ] Performance profiling
- [ ] Platform-adaptive layouts

---

## Performance Targets

| Metric | Target |
|--------|--------|
| Frame Rate | 60 FPS stable |
| Active Widgets | < 100 (pooled) |
| Draw Calls | < 20 per frame |
| UI Memory | < 180 MB |
| CPU Time | < 2ms per frame |

---

## Built With

- **Unreal Engine 5.7**
- **C++17**
- **UMG (Unreal Motion Graphics)**
- **Blueprint Visual Scripting**

---

## Author

**Rachel** — UI Engineer & Game Designer

- Practicing UE5 C++ development
- Background in UI Programming for games and Front End Web Development

---

## License

TBD

---

## Acknowledgments

- Epic Games documentation on [Programming Subsystems](https://docs.unrealengine.com/5.0/en-US/programming-subsystems-in-unreal-engine/)
- Epic's [UMG Best Practices](https://www.unrealengine.com/en-US/blog/umg-best-practices)
- Fortnite UI as design reference
