// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "fuchsia/runners/cast/cast_runner.h"

#include <fuchsia/sys/cpp/fidl.h>
#include <memory>
#include <string>
#include <utility>

<<<<<<< HEAD
#include "base/logging.h"
#include "base/memory/ptr_util.h"
=======
#include "base/fuchsia/fuchsia_logging.h"
#include "base/fuchsia/startup_context.h"
#include "base/logging.h"
<<<<<<< HEAD
#include "base/memory/ptr_util.h"
#include "base/process/process.h"
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
=======
#include "fuchsia/base/agent_manager.h"
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5
#include "fuchsia/runners/cast/cast_component.h"
#include "url/gurl.h"

CastRunner::CastRunner(base::fuchsia::ServiceDirectory* service_directory,
                       chromium::web::ContextPtr context,
                       base::OnceClosure on_idle_closure)
    : WebContentRunner(service_directory,
                       std::move(context),
<<<<<<< HEAD
                       std::move(on_idle_closure)),
<<<<<<< HEAD
      app_config_manager_(std::move(app_config_manager)) {}
=======
      app_config_manager_(std::move(app_config_manager)) {
  app_config_manager_.set_error_handler([](zx_status_t status) {
    ZX_LOG(WARNING, status) << "ApplicationConfigManager disconnected";
  });
}
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
=======
                       std::move(on_idle_closure)) {}
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5

CastRunner::~CastRunner() = default;

struct CastRunner::PendingComponent {
  chromium::cast::ApplicationConfigManagerPtr app_config_manager;
  std::unique_ptr<base::fuchsia::StartupContext> startup_context;
  std::unique_ptr<cr_fuchsia::AgentManager> agent_manager;
  fidl::InterfaceRequest<fuchsia::sys::ComponentController> controller_request;
};

void CastRunner::StartComponent(
    fuchsia::sys::Package package,
    fuchsia::sys::StartupInfo startup_info,
    fidl::InterfaceRequest<fuchsia::sys::ComponentController>
        controller_request) {
  // Verify that |package| specifies a Cast URI, and pull the app-Id from it.
  constexpr char kCastPresentationUrlScheme[] = "cast";
  constexpr char kCastSecurePresentationUrlScheme[] = "casts";

  GURL cast_url(package.resolved_url);
  if (!cast_url.is_valid() ||
      (!cast_url.SchemeIs(kCastPresentationUrlScheme) &&
       !cast_url.SchemeIs(kCastSecurePresentationUrlScheme)) ||
      cast_url.GetContent().empty()) {
    LOG(ERROR) << "Rejected invalid URL: " << cast_url;
    return;
  }

<<<<<<< HEAD
  // Fetch the Cast application configuration for the specified Id.
  const std::string cast_app_id(cast_url.GetContent());
<<<<<<< HEAD
  app_config_manager_->GetConfig(
      cast_app_id,
      [this,
       startup_context = std::make_unique<base::fuchsia::StartupContext>(
           std::move(startup_info)),
=======

  // TODO(https://crbug.com/933831): Look for ApplicationConfigManager in the
  // per-component incoming services. This works-around an issue with binding
  // to that service via the Runner's incoming services. Replace this with a
  // request for services from a Cast-specific Agent.
  auto startup_context =
=======
  // The application configuration asynchronously via the per-component
  // ApplicationConfigManager, the pointer to that service must be kept live
  // until the request completes, or CastRunner is deleted.
  auto pending_component = std::make_unique<PendingComponent>();
  pending_component->startup_context =
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5
      std::make_unique<base::fuchsia::StartupContext>(std::move(startup_info));
  pending_component->agent_manager = std::make_unique<cr_fuchsia::AgentManager>(
      pending_component->startup_context->incoming_services());
  pending_component->controller_request = std::move(controller_request);

  // Request the configuration for the specified application.
  pending_component->agent_manager->ConnectToAgentService(
      kAgentComponentUrl, pending_component->app_config_manager.NewRequest());
  pending_component->app_config_manager.set_error_handler(
      [this, pending_component = pending_component.get()](zx_status_t status) {
        ZX_LOG(ERROR, status) << "ApplicationConfigManager disconnected.";
        GetConfigCallback(pending_component,
                          chromium::cast::ApplicationConfig());
      });

<<<<<<< HEAD
  app_config_manager_->GetConfig(
      cast_app_id,
      [this, startup_context = std::move(startup_context),
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
       controller_request = std::move(controller_request)](
          chromium::cast::ApplicationConfigPtr app_config) mutable {
        GetConfigCallback(std::move(startup_context),
                          std::move(controller_request), std::move(app_config));
=======
  const std::string cast_app_id(cast_url.GetContent());
  pending_component->app_config_manager->GetConfig(
      cast_app_id, [this, pending_component = pending_component.get()](
                       chromium::cast::ApplicationConfig app_config) {
        GetConfigCallback(pending_component, std::move(app_config));
>>>>>>> 2d57e5b8afc6d01b344a8d95d3470d46b35845c5
      });

  pending_components_.emplace(std::move(pending_component));
}

const char CastRunner::kAgentComponentUrl[] =
    "fuchsia-pkg://fuchsia.com/cast_agent#meta/cast_agent.cmx";

void CastRunner::GetConfigCallback(
    PendingComponent* pending_component,
    chromium::cast::ApplicationConfig app_config) {
  // Ideally the PendingComponent would be move()d out of |pending_components_|
  // here, but that requires extract(), which isn't available until C++17.
  // Instead find |pending_component| and move() the individual fields out
  // before erase()ing it.
  auto it = pending_components_.find(pending_component);
  DCHECK(it != pending_components_.end());

  // If no configuration was returned then ignore the request.
  if (!app_config.has_web_url()) {
    pending_components_.erase(it);
    DLOG(WARNING) << "No ApplicationConfig was found.";
<<<<<<< HEAD

    // For test purposes, we need to call RegisterComponent even if there is no
    // URL to launch.
    // TODO: Replace this hack, e.g. with an test-specific callback.
    RegisterComponent(std::unique_ptr<WebComponent>(nullptr));
=======
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
    return;
  }

  // Create a component based on the returned configuration, and pass it the
  // fields stashed in PendingComponent.
  GURL cast_app_url(app_config.web_url());
  auto component = std::make_unique<CastComponent>(
      this, std::move(pending_component->startup_context),
      std::move(pending_component->controller_request),
      std::move(pending_component->agent_manager));
  pending_components_.erase(it);

  // Disable input for the Frame by default.
  component->frame()->SetEnableInput(false);

  component->LoadUrl(std::move(cast_app_url));
  RegisterComponent(std::move(component));
}
