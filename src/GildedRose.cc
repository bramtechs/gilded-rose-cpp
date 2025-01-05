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
                item.quality = item.quality - 1;
            }
        } else if (item.quality < 50) {
            item.quality = item.quality + 1;

            if (isBackstagePass(item)) {
                if (item.sellIn < 11 && item.quality < 50) {
                    item.quality = item.quality + 1;
                }

                if (item.sellIn < 6 && item.quality < 50) {
                    item.quality = item.quality + 1;
                }
            }
        }

        if (item.name != "Sulfuras, Hand of Ragnaros") {
            item.sellIn = item.sellIn - 1;
        }

        if (item.sellIn < 0) {
            if (!isAgedBrie(item)) {
                if (!isBackstagePass(item)) {
                    if (item.quality > 0 && item.name != "Sulfuras, Hand of Ragnaros") {
                        item.quality = item.quality - 1;
                    }
                } else {
                    item.quality = item.quality - item.quality;
                }
            } else if (item.quality < 50) {
                item.quality = item.quality + 1;
            }
        }
    }
}
