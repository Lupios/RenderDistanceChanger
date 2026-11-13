// RenderDistanceKeys - LeviLauncher Native Mod
#include <algorithm>
#include <atomic>

namespace rdk {

constexpr int kKeyLeft  = 21;
constexpr int kKeyRight = 22;
constexpr int kMinChunks = 4;
constexpr int kMaxChunks = 96;
constexpr int kStep      = 2;

std::atomic<int> gChunks{12};

// TODO: versionsspezifische Funktion zum Setzen der Render Distance
bool applyRenderDistance(int) {
    return false;
}

void changeBy(int delta) {
    int v = std::clamp(gChunks.load() + delta, kMinChunks, kMaxChunks);
    gChunks = v;
    applyRenderDistance(v);
}

bool onKey(int keyCode, bool down) {
    if (!down) return false;
    if (keyCode == kKeyLeft)  { changeBy(-kStep); return true; }
    if (keyCode == kKeyRight) { changeBy(+kStep); return true; }
    return false;
}

}  // namespace rdk
