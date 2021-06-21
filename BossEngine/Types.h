#pragma once
#include <limits>
#include <cstdint>
#include <bitset>

// ECS
using Entity = std::uint64_t;
const Entity MAX_ENTITIES = 10000;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

// Event System
// Events
using EventId = std::uint64_t;
using ParamId = std::uint64_t;