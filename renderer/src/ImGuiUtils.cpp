#include "pch.h"

#include "ImGuiUtils.h"


float ImGuiUtils::TableHeight(const int rowCount, const int maxRowCount) {
    const float rowHeight = ImGui::GetTextLineHeightWithSpacing() + ImGui::GetStyle().CellPadding.y * 2.0f;
    const float headerHeight = ImGui::GetFrameHeightWithSpacing();
    const int count = std::min(rowCount, maxRowCount);
    return headerHeight + static_cast<float>(count) * rowHeight + ImGui::GetStyle().ItemSpacing.y * 2.0f + 10.0f;
}
