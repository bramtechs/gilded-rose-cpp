#include "GildedRose.h"

GildedRose::GildedRose(std::vector<Item>& items)
    : items(items)
{
}

bool GildedRose::isAgedBrie(const Item& item) const
{
    return item.name == "Aged Brie";
}

bool GildedRose::isBackstagePass(const Item& item) const
{
    return item.name.find("Backstage pass") != std::string::npos;
}

void GildedRose::updateQuality()
{
    for (Item& item : items) {
        if (!isAgedBrie(item) && !isBackstagePass(item)) {
            if (item.quality > 0 && item.name != "Sulfuras, Hand of Ragnaros") {
                item.quality--;
            }
        } else if (item.quality < 50) {
            item.quality++;

            if (isBackstagePass(item)) {
                if (item.sellIn < 11 && item.quality < 50) {
                    item.quality++;
                }

                if (item.sellIn < 6 && item.quality < 50) {
                    item.quality++;
                }
            }
        }

        if (item.name != "Sulfuras, Hand of Ragnaros") {
            item.sellIn--;
        }

        if (item.sellIn < 0) {
            if (!isAgedBrie(item)) {
                if (!isBackstagePass(item)) {
                    if (item.quality > 0 && item.name != "Sulfuras, Hand of Ragnaros") {
                        item.quality--;
                    }
                } else {
                    item.quality -= item.quality;
                }
            } else if (item.quality < 50) {
                item.quality++;
            }
        }
    }
}
