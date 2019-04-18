// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ash/shelf/shelf_control_button.h"

#include "ash/public/cpp/ash_constants.h"
#include "ash/resources/vector_icons/vector_icons.h"
#include "ash/shelf/ink_drop_button_listener.h"
#include "ash/shelf/shelf.h"
#include "ash/shelf/shelf_constants.h"
#include "ash/shelf/shelf_view.h"
#include "ash/system/tray/tray_popup_utils.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/paint_vector_icon.h"
#include "ui/views/animation/flood_fill_ink_drop_ripple.h"
#include "ui/views/animation/ink_drop_impl.h"
#include "ui/views/animation/ink_drop_mask.h"
#include "ui/views/view_class_properties.h"
#include "ui/views/widget/widget.h"

namespace ash {

ShelfControlButton::ShelfControlButton(ShelfView* shelf_view)
    : ShelfButton(shelf_view), shelf_(shelf_view->shelf()) {
  set_has_ink_drop_action_on_click(true);

  SetInstallFocusRingOnFocus(true);
  focus_ring()->SetColor(kShelfFocusBorderColor);
  SetFocusPainter(nullptr);
}

ShelfControlButton::~ShelfControlButton() = default;

gfx::Point ShelfControlButton::GetCenterPoint() const {
  return gfx::Point(width() / 2.f, height() / 2.f);
}

std::unique_ptr<views::InkDropRipple> ShelfControlButton::CreateInkDropRipple()
    const {
  const int button_radius = ShelfConstants::control_border_radius();
  gfx::Point center = GetCenterPoint();
  gfx::Rect bounds(center.x() - button_radius, center.y() - button_radius,
                   2 * button_radius, 2 * button_radius);
  return std::make_unique<views::FloodFillInkDropRipple>(
      size(), GetLocalBounds().InsetsFrom(bounds),
      GetInkDropCenterBasedOnLastEvent(), GetInkDropBaseColor(),
      ink_drop_visible_opacity());
}

std::unique_ptr<views::InkDropMask> ShelfControlButton::CreateInkDropMask()
    const {
  return std::make_unique<views::CircleInkDropMask>(
      size(), GetCenterPoint(), ShelfConstants::control_border_radius());
}

<<<<<<< HEAD
=======
const char* ShelfControlButton::GetClassName() const {
  return "ash/ShelfControlButton";
}

<<<<<<< HEAD
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
gfx::Rect ShelfControlButton::CalculateButtonBounds() const {
  ShelfAlignment alignment = shelf_->alignment();
  gfx::Rect content_bounds = GetContentsBounds();
  // Align the button to the top of a bottom-aligned shelf, to the right edge
  // a left-aligned shelf, and to the left edge of a right-aligned shelf.
  const int inset = (ShelfConstants::shelf_size() - kShelfControlSize) / 2;
  const int x = alignment == SHELF_ALIGNMENT_LEFT
                    ? content_bounds.right() - inset - kShelfControlSize
                    : content_bounds.x() + inset;
  return gfx::Rect(x, content_bounds.y() + inset, kShelfControlSize,
                   kShelfControlSize);
=======
void ShelfControlButton::PaintButtonContents(gfx::Canvas* canvas) {
  PaintBackground(canvas, GetContentsBounds());
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5
}

void ShelfControlButton::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  const int border_radius = ShelfConstants::control_border_radius();
  // Some control buttons have a slightly larger size to fill the shelf and
  // maximize the click target, but we still want their "visual" size to be
  // the same, so we find the center point and draw a square around that.
  const gfx::Point center = GetCenterPoint();
  const int half_size = kShelfControlSize / 2;
  const gfx::Rect visual_size(center.x() - half_size, center.y() - half_size,
                              kShelfControlSize, kShelfControlSize);
  auto path = std::make_unique<SkPath>();
  path->addRoundRect(gfx::RectToSkRect(visual_size), border_radius,
                     border_radius);
  SetProperty(views::kHighlightPathKey, path.release());
  ShelfButton::OnBoundsChanged(previous_bounds);
}

void ShelfControlButton::PaintBackground(gfx::Canvas* canvas,
                                         const gfx::Rect& bounds) {
  cc::PaintFlags flags;
  flags.setAntiAlias(true);
  flags.setColor(kShelfControlPermanentHighlightBackground);
  canvas->DrawRoundRect(bounds, ShelfConstants::control_border_radius(), flags);
}

}  // namespace ash
