#include "gui.hpp"

#include "liquid-status-window.hpp"

#include <libdevice/device.hpp>
#include <libmechanism/mechanism.hpp>

#include "util.hpp"

NAMESPACE_BEGIN

namespace gui {
LiquidStatusWindow::LiquidStatusWindow(float                   width,
                                       float                   height,
                                       const ImGuiWindowFlags& flags)
    : Window{"Liquid Status", width, height, flags} {}

LiquidStatusWindow::~LiquidStatusWindow() {}

void LiquidStatusWindow::show([[maybe_unused]] Manager* manager) {
  massert(Config::get() != nullptr, "sanity");
  massert(mechanism::LiquidRefilling::get() != nullptr, "sanity");
  massert(mechanism::LiquidRefilling::get()->active(), "sanity");

  auto* liquid_refilling = mechanism::LiquidRefilling::get();

  unsigned int status_id = 0;

  const ImVec2 size = util::size::h_wide(32.0f);

  ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.8f);

  ImGui::Columns(2, NULL, /* v_borders */ true);
  {
    const mechanism::liquid::status status = liquid_refilling->water_level();

    if (ImGui::GetColumnIndex() == 0)
      ImGui::Separator();

    ImGui::Text("WATER");
    util::status_button("REFILLING", status_id++, false, size);
    util::status_button("FULL", status_id++,
                        status == mechanism::liquid::status::full, size);
    util::status_button("NORMAL", status_id++,
                        status == mechanism::liquid::status::normal, size);
    util::status_button("EMPTY", status_id++,
                        status == mechanism::liquid::status::empty, size);
  }
  ImGui::NextColumn();
  {
    const mechanism::liquid::status status =
        liquid_refilling->disinfectant_level();

    ImGui::Text("DISINFECTANT");
    util::status_button("REFILLING", status_id++, false, size);
    util::status_button("FULL", status_id++,
                        status == mechanism::liquid::status::full, size);
    util::status_button("NORMAL", status_id++,
                        status == mechanism::liquid::status::normal, size);
    util::status_button("EMPTY", status_id++,
                        status == mechanism::liquid::status::empty, size);
  }
  ImGui::NextColumn();

  ImGui::PopStyleVar();
}
}  // namespace gui

NAMESPACE_END