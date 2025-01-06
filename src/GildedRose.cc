/*
 * Copyright (c) 2025. Doomhowl Interactive - bramtechs/brambasiel
 * MIT License. Absolutely no warranty.
 *
 * File created on: 05-01-2025
 */

#include "GildedRose.h"
#include "unsafe_variant.hh"

/**
 Application of the **strategy** pattern.
 After evaluation, it is not the command pattern.
 */
class IQualityUpdater {
public:
    virtual ~IQualityUpdater() = default;
    virtual void execute(Item& item) = 0;
};

class AgedBrieQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        if (item.quality < 50) {
            item.quality++;
        }
        if (item.sellIn < 0 && item.quality < 50) {
            item.quality++;
        }
        item.sellIn--;
    }
};

class BackstagePassQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        item.quality++;

        if (item.quality < 50) {
            if (item.sellIn <= 10) {
                item.quality++;
            }

            if (item.sellIn <= 5) {
                item.quality++;
            }
        }
        item.sellIn--;
        if (item.sellIn < 0) {
            item.quality = 0;
        }
    }
};

class SulfurasQualityUpdater final : public IQualityUpdater {
public:
    void execute(Item& item) override
    {
        // Sulfuras never has to be sold or decreases in quality
    }
};

class NormalQualityUpdater : public IQualityUpdater {
public:
    NormalQualityUpdater(int qualityDecreaseStep = 1)
        : mQualityDecreaseStep(qualityDecreaseStep)
    {
    }

    void execute(Item& item) override
    {
        if (item.quality > 0) {
            item.quality -= mQualityDecreaseStep;
        }
        item.sellIn--;
        if (item.sellIn < 0 && item.quality > 0) {
            item.quality -= mQualityDecreaseStep;
        }
    }

private:
    int mQualityDecreaseStep;
};

class ConjuredQualityUpdater final : public NormalQualityUpdater {
public:
    ConjuredQualityUpdater()
        : NormalQualityUpdater(2)
    {
    }
};

// Variant of all quality updaters. Quite overkill for this exercise,
// but avoids heap allocation for every item quality update.
using AnyQualityUpdaters = howl::unsafe_variant<
    AgedBrieQualityUpdater,
    BackstagePassQualityUpdater,
    SulfurasQualityUpdater,
    ConjuredQualityUpdater,
    NormalQualityUpdater>;

class QualityUpdaterFactory {
public:
    AnyQualityUpdaters create(const Item& item)
    {
        if (isAgedBrie(item)) {
            return AgedBrieQualityUpdater();
        }
        if (isBackstagePass(item)) {
            return BackstagePassQualityUpdater();
        }
        if (isHandOfRagnaros(item)) {
            return SulfurasQualityUpdater();
        }
        if (isConjured(item)) {
            return ConjuredQualityUpdater();
        }
        return NormalQualityUpdater();
    }

private:
    bool isAgedBrie(const Item& item) const
    {
        return item.name == "Aged Brie";
    }

    bool isBackstagePass(const Item& item) const
    {
        return item.name.find("Backstage pass") != std::string::npos;
    }

    bool isHandOfRagnaros(const Item& item) const
    {
        return item.name == "Sulfuras, Hand of Ragnaros";
    }

    bool isConjured(const Item& item) const
    {
        return item.name.find("Conjured") != std::string::npos;
    }
};

GildedRose::GildedRose(std::vector<Item>& items)
    : items(items)
{
}

void GildedRose::updateQuality()
{
    QualityUpdaterFactory factory {};
    for (Item& item : items) {
        auto& updater = factory.create(item).get<IQualityUpdater>();
        updater.execute(item);
    }
}
