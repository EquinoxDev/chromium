// Copyright 2017 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef THIRD_PARTY_BLINK_RENDERER_MODULES_XR_XR_FRAME_H_
#define THIRD_PARTY_BLINK_RENDERER_MODULES_XR_XR_FRAME_H_

#include <memory>

#include "device/vr/public/mojom/vr_service.mojom-blink.h"
#include "third_party/blink/renderer/platform/bindings/script_wrappable.h"
#include "third_party/blink/renderer/platform/heap/handle.h"
#include "third_party/blink/renderer/platform/transforms/transformation_matrix.h"
#include "third_party/blink/renderer/platform/wtf/forward.h"

namespace blink {

class ExceptionState;
class XRInputSource;
class XRPose;
class XRReferenceSpace;
class XRSession;
class XRSpace;
class XRViewerPose;

class XRFrame final : public ScriptWrappable {
  DEFINE_WRAPPERTYPEINFO();

 public:
  explicit XRFrame(XRSession*);

  XRSession* session() const { return session_; }

<<<<<<< HEAD
  XRViewerPose* getViewerPose(XRReferenceSpace*) const;
  XRInputPose* getInputPose(XRInputSource*, XRReferenceSpace*) const;
=======
  XRViewerPose* getViewerPose(XRReferenceSpace*, ExceptionState&) const;
<<<<<<< HEAD
  XRInputPose* getInputPose(XRInputSource*,
                            XRReferenceSpace*,
                            ExceptionState&) const;
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
=======
  XRPose* getPose(XRSpace*, XRSpace*, ExceptionState&);
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5

  void SetBasePoseMatrix(const TransformationMatrix&);
  std::unique_ptr<TransformationMatrix> CloneBasePoseMatrix() const;

  void Trace(blink::Visitor*) override;

  void Deactivate();

  void SetAnimationFrame(bool is_animation_frame) {
    is_animation_frame_ = is_animation_frame;
  }

 private:
  std::unique_ptr<TransformationMatrix> GetAdjustedPoseMatrix(XRSpace*) const;
  XRPose* GetTargetRayPose(XRInputSource*, XRSpace*) const;
  XRPose* GetGripPose(XRInputSource*, XRSpace*) const;

  const Member<XRSession> session_;

  // Maps from mojo space to headset space.
  std::unique_ptr<TransformationMatrix> base_pose_matrix_;

  // Frames are only active during callbacks. getPose and getViewerPose should
  // only be called from JS on active frames.
  bool is_active_ = true;

  // Only frames created by XRSession.requestAnimationFrame callbacks are
  // animation frames. getViewerPose should only be called from JS on active
  // animation frames.
  bool is_animation_frame_ = false;
};

}  // namespace blink

#endif  // THIRD_PARTY_BLINK_RENDERER_MODULES_XR_XR_FRAME_H_
