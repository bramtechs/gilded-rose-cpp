#include <catch2/catch.hpp>

#include "GildedRose.h"

std::ostream& operator<<(std::ostream& os, const Item& obj)
{
    return os
        << "name: " << obj.name
        << ", sellIn: " << obj.sellIn
        << ", quality: " << obj.quality;
}

TEST_CASE("Check item name")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 0));
    GildedRose app(items);
    REQUIRE("foo" == app.items[0].name);
}

TEST_CASE("UpdateQuality should decrease sellIn by one")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 0));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(-1 == app.items[0].sellIn);
}

TEST_CASE("UpdateQualty should decrease quality by one")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 1, 1));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(0 == app.items[0].quality);
}

TEST_CASE("Negative sell by date, quality degrades twice as fast")
{
    std::vector<Item> items;
    items.push_back(Item("foo", 0, 10));
    GildedRose app(items);
    app.updateQuality();
    REQUIRE(8 == app.items[0].quality);
}
