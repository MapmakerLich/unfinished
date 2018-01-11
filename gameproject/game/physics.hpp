#pragma once

#include "main.hpp"

void jumping(Character &character, float deltaTime);

void flying(Grenade &grenade, Canon &canon, float deltaTime);

void update(Character &character, Grenade &grenade, Canon &canon, float deltaTime, std::vector<EasyBot> &vectorOfBots, Texts &texts, Platform &platform);