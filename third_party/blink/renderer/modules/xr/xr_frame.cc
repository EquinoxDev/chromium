// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "third_party/blink/renderer/modules/xr/xr_frame.h"

#include "third_party/blink/renderer/core/dom/dom_exception.h"
#include "third_party/blink/renderer/modules/xr/xr_input_source.h"
#include "third_party/blink/renderer/modules/xr/xr_reference_space.h"
#include "third_party/blink/renderer/modules/xr/xr_session.h"
#include "third_party/blink/renderer/modules/xr/xr_view.h"
#include "third_party/blink/renderer/modules/xr/xr_viewer_pose.h"
#include "third_party/blink/renderer/platform/bindings/exception_state.h"

namespace blink {

namespace {

<<<<<<< HEAD
XRViewerPose* XRFrame::getViewerPose(XRReferenceSpace* reference_space) const {
  session_->LogGetPose();

  // Must use a reference space created from the same session.
  if (!reference_space || reference_space->session() != session_) {
    return nullptr;
  }

  std::unique_ptr<TransformationMatrix> pose;

=======
const char kInactiveFrame[] =
    "XRFrame access outside the callback that produced it is invalid.";

const char kNonAnimationFrame[] =
    "getViewerPose can only be called on XRFrame objects passed to "
    "XRSession.requestAnimationFrame callbacks.";

const char kSessionMismatch[] = "XRSpace and XRFrame sessions do not match.";

}  // namespace

XRFrame::XRFrame(XRSession* session) : session_(session) {}

std::unique_ptr<TransformationMatrix> XRFrame::CloneBasePoseMatrix() const {
  if (!base_pose_matrix_) {
    return nullptr;
  }

  return std::make_unique<TransformationMatrix>(*base_pose_matrix_);
}

XRViewerPose* XRFrame::getViewerPose(XRReferenceSpace* reference_space,
                                     ExceptionState& exception_state) const {
  if (!is_active_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kInactiveFrame);
    return nullptr;
  }

  if (!is_animation_frame_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kNonAnimationFrame);
    return nullptr;
  }

<<<<<<< HEAD
  std::unique_ptr<TransformationMatrix> pose;

>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  // If we don't have a valid base pose request the reference space's default
  // pose. Most common when tracking is lost.
  if (base_pose_matrix_) {
    pose = reference_space->TransformBasePose(*base_pose_matrix_);
  } else {
    pose = reference_space->DefaultPose();
=======
  if (!reference_space) {
    return nullptr;
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5
  }

  // Must use a reference space created from the same session.
  if (reference_space->session() != session_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kSessionMismatch);
    return nullptr;
  }

  session_->LogGetPose();

  std::unique_ptr<TransformationMatrix> pose =
      reference_space->GetViewerPoseMatrix(CloneBasePoseMatrix());
  if (!pose) {
    return nullptr;
  }

  return MakeGarbageCollected<XRViewerPose>(session(), std::move(pose));
}

// Return an XRPose that has a transform mapping to space A from space B, while
// accounting for the base pose matrix of this frame. If computing a transform
// isn't possible, return nullptr.
XRPose* XRFrame::getPose(XRSpace* space_A,
                         XRSpace* space_B,
                         ExceptionState& exception_state) {
  if (!is_active_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kInactiveFrame);
    return nullptr;
  }

  if (!space_A || !space_B) {
    return nullptr;
  }

  if (space_A->session() != session_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kSessionMismatch);
    return nullptr;
  }

  if (space_B->session() != session_) {
    exception_state.ThrowDOMException(DOMExceptionCode::kInvalidStateError,
                                      kSessionMismatch);
    return nullptr;
  }

  return space_A->getPose(space_B, CloneBasePoseMatrix());
}

void XRFrame::SetBasePoseMatrix(const TransformationMatrix& base_pose_matrix) {
  base_pose_matrix_ = std::make_unique<TransformationMatrix>(base_pose_matrix);
}

void XRFrame::Deactivate() {
  is_active_ = false;
  is_animation_frame_ = false;
}

void XRFrame::Trace(blink::Visitor* visitor) {
  visitor->Trace(session_);
  ScriptWrappable::Trace(visitor);
}

}  // namespace blink
