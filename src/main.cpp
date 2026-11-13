#include <algorithm>
#include <atomic>
#include <string>
#include <string_view>

#include <pl/Mod.hpp>
#include <pl/ModMenu.hpp>

namespace {

constexpr const char *kModuleId = "render_distance_changer.module";
constexpr const char *kMinusId = "render_distance_changer.minus";
constexpr const char *kPlusId = "render_distance_changer.plus";
constexpr const char *kDistanceKey = "distance";
constexpr int kMin = 4;
constexpr int kMax = 96;
constexpr int kStep = 2;

std::atomic<int> gChunks{12};

// TODO: Hier kommt der echte Aufruf, der die Render Distance im Spiel setzt.
// Das braucht eine Signatur fuer deine Minecraft-Version.
bool applyRenderDistance(int chunks) {
  (void)chunks;
  return false;
}

class RenderDistanceChanger {
public:
  static RenderDistanceChanger &instance() {
    static RenderDistanceChanger mod;
    return mod;
  }

  RenderDistanceChanger() : mSelf(*ll::mod::NativeMod::current()) {}

  bool load() { return true; }

  bool enable() {
    const bool moduleOk =
        pl::modmenu::ModuleBuilder(kModuleId, "Render Distance")
            .modId(mSelf.getId())
            .description("Render Distance per Button aendern")
            .defaultEnabled(true)
            .onToggle(onModuleToggle)
            .config(kDistanceKey, "Render Distance",
                    pl::modmenu::ConfigType::SliderInt, std::to_string(12),
                    std::to_string(kMin), std::to_string(kMax))
            .onConfigChanged(onConfigChanged)
            .registerModule();

    const bool minusOk =
        moduleOk &&
        pl::modmenu::ButtonBuilder(kMinusId, "Render Distance minus")
            .moduleId(kModuleId)
            .label("-")
            .behavior(pl::modmenu::ButtonBehavior::Click)
            .onEvent(onButtonEvent)
            .registerButton();

    const bool plusOk =
        moduleOk &&
        pl::modmenu::ButtonBuilder(kPlusId, "Render Distance plus")
            .moduleId(kModuleId)
            .label("+")
            .behavior(pl::modmenu::ButtonBehavior::Click)
            .onEvent(onButtonEvent)
            .registerButton();

    mSelf.getLogger().info("Registriert: module={} minus={} plus={}",
                           moduleOk, minusOk, plusOk);
    return moduleOk && minusOk && plusOk;
  }

  bool disable() {
    unregisterAll();
    return true;
  }

  bool unload() {
    unregisterAll();
    return true;
  }

private:
  ll::mod::NativeMod &mSelf;

  void change(int newValue) {
    const int v = std::clamp(newValue, kMin, kMax);
    gChunks = v;
    const bool ok = applyRenderDistance(v);
    mSelf.getLogger().info("Render Distance: {} (angewendet={})", v, ok);
  }

  static void onModuleToggle(std::string_view, bool) {}

  static void onConfigChanged(std::string_view moduleId, std::string_view key,
                              std::string_view value) {
    if (moduleId != kModuleId || key != kDistanceKey) {
      return;
    }
    const std::string text(value);
    instance().change(std::atoi(text.c_str()));
  }

  static void onButtonEvent(std::string_view buttonId,
                            pl::modmenu::ButtonEvent event, float) {
    if (event != pl::modmenu::ButtonEvent::Down) {
      return;
    }
    auto &self = instance();
    if (buttonId == kMinusId) {
      self.change(gChunks.load() - kStep);
    } else if (buttonId == kPlusId) {
      self.change(gChunks.load() + kStep);
    }
  }

  void unregisterAll() {
    pl::modmenu::unregisterButton(kMinusId);
    pl::modmenu::unregisterButton(kPlusId);
    pl::modmenu::unregisterModule(kModuleId);
  }
};

} // namespace

PL_REGISTER_MOD(RenderDistanceChanger, RenderDistanceChanger::instance())
