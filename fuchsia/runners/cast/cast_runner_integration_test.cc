// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <lib/fidl/cpp/binding.h>
#include <lib/zx/channel.h>

#include "base/fuchsia/fuchsia_logging.h"
<<<<<<< HEAD
=======
#include "base/fuchsia/scoped_service_binding.h"
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
#include "base/fuchsia/service_directory.h"
#include "base/fuchsia/service_directory_client.h"
#include "base/message_loop/message_loop.h"
#include "base/strings/stringprintf.h"
<<<<<<< HEAD
=======
#include "base/test/bind_test_util.h"
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
#include "base/test/test_timeouts.h"
#include "fuchsia/base/fit_adapter.h"
#include "fuchsia/base/result_receiver.h"
#include "fuchsia/engine/test/test_common.h"
#include "fuchsia/fidl/chromium/web/cpp/fidl.h"
#include "fuchsia/runners/cast/cast_runner.h"
#include "fuchsia/runners/cast/fake_application_config_manager.h"
<<<<<<< HEAD
#include "fuchsia/runners/cast/test_common.h"
=======
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
#include "fuchsia/runners/common/web_component.h"
#include "fuchsia/runners/common/web_content_runner.h"
#include "net/test/embedded_test_server/default_handlers.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace castrunner {

namespace {

const char kTestServerRoot[] =
    FILE_PATH_LITERAL("fuchsia/runners/cast/testdata");

void ComponentErrorHandler(zx_status_t status) {
<<<<<<< HEAD
  ZX_LOG(ERROR, status) << "Component launch failed.";
  ADD_FAILURE();
}

}  // namespace

class CastRunnerIntegrationTest : public testing::Test,
                                  public chromium::cast::CastChannel {
 public:
  CastRunnerIntegrationTest()
      : run_timeout_(TestTimeouts::action_timeout()),
        cast_channel_binding_(this) {
    // Create a new test ServiceDirectory, and ServiceDirectoryClient connected
    // to it, for tests to use to drive the CastRunner.
    fidl::InterfaceHandle<fuchsia::io::Directory> directory;
    test_services_ = std::make_unique<base::fuchsia::ServiceDirectory>(
        directory.NewRequest());
    test_services_client_ =
        std::make_unique<base::fuchsia::ServiceDirectoryClient>(
            std::move(directory));

    // Create the AppConfigManager.
    app_config_binding_ = std::make_unique<
        fidl::Binding<chromium::cast::ApplicationConfigManager>>(
        &app_config_manager_);
    chromium::cast::ApplicationConfigManagerPtr app_config_manager_interface;
    app_config_binding_->Bind(app_config_manager_interface.NewRequest());

    // Create the CastRunner, published into |test_services_|.
    cast_runner_ = std::make_unique<CastRunner>(
        test_services_.get(), WebContentRunner::CreateDefaultWebContext(),
        std::move(app_config_manager_interface),
        cast_runner_run_loop_.QuitClosure());

    // Connect to the CastRunner's fuchsia.sys.Runner interface.
    cast_runner_ptr_ =
        test_services_client_->ConnectToService<fuchsia::sys::Runner>();
=======
  ZX_LOG(ERROR, status) << "Component launch failed";
  ADD_FAILURE();
}

class FakeCastChannel : public chromium::cast::CastChannel {
 public:
  FakeCastChannel() = default;

  // chromium::web::CastChannel implementation.
  void OnOpened(fidl::InterfaceHandle<chromium::web::MessagePort> channel,
                OnOpenedCallback callback_ignored) override {
    // |callback_ignored| is dropped because these tests don't exercise multiple
    // channel lifetimes.
    connected_channel_ = channel.Bind();

    if (on_channel_connected_)
      std::move(on_channel_connected_).Run();
  }

  chromium::web::MessagePort* connected_channel() const {
    // Return nullptr if |connected_channel_| is not currently bound.
    return connected_channel_ ? connected_channel_.get() : nullptr;
  }

  void set_on_channel_connected(base::OnceClosure on_channel_connected) {
    on_channel_connected_ = std::move(on_channel_connected);
  }

 protected:
  // The connected Cast Channel.
  chromium::web::MessagePortPtr connected_channel_;

  // A pending on-connect callback, to be invoked once a Cast Channel is
  // received.
  base::OnceClosure on_channel_connected_;

  DISALLOW_COPY_AND_ASSIGN(FakeCastChannel);
};

}  // namespace

class CastRunnerIntegrationTest : public testing::Test {
 public:
  CastRunnerIntegrationTest()
      : run_timeout_(TestTimeouts::action_timeout()),
        app_config_binding_(&app_config_manager_) {
    // Create a new test ServiceDirectory, and ServiceDirectoryClient connected
    // to it, for tests to use to drive the CastRunner.
    fidl::InterfaceHandle<fuchsia::io::Directory> directory;
    public_services_ = std::make_unique<base::fuchsia::ServiceDirectory>(
        directory.NewRequest());

    // Create the AppConfigManager.
    chromium::cast::ApplicationConfigManagerPtr app_config_manager_ptr;
    app_config_binding_.Bind(app_config_manager_ptr.NewRequest());

    // Create the CastRunner, published into |test_services_|.
    cast_runner_ = std::make_unique<CastRunner>(
        public_services_.get(), WebContentRunner::CreateDefaultWebContext(),
        std::move(app_config_manager_ptr), cast_runner_run_loop_.QuitClosure());

    // Connect to the CastRunner's fuchsia.sys.Runner interface.
    base::fuchsia::ServiceDirectoryClient public_directory_client(
        std::move(directory));
    cast_runner_ptr_ =
        public_directory_client.ConnectToService<fuchsia::sys::Runner>();
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
    cast_runner_ptr_.set_error_handler([this](zx_status_t status) {
      ZX_LOG(ERROR, status) << "CastRunner closed channel.";
      ADD_FAILURE();
      cast_runner_run_loop_.Quit();
    });
  }

  void SetUp() override {
    test_server_.ServeFilesFromSourceDirectory(kTestServerRoot);
    net::test_server::RegisterDefaultHandlers(&test_server_);
    ASSERT_TRUE(test_server_.Start());
  }

  void TearDown() override {
    // Disconnect the CastRunner.
    cast_runner_ptr_.Unbind();
    cast_runner_run_loop_.Run();
  }

  void WaitUntilCastChannelOpened() {
<<<<<<< HEAD
    if (connected_channel_)
      return;

    base::RunLoop run_loop;
    on_channel_connected_cb_ = run_loop.QuitClosure();
    run_loop.Run();
  }

  // chromium::web::CastChannel implementation.
  void OnOpened(fidl::InterfaceHandle<chromium::web::MessagePort> channel,
                OnOpenedCallback callback_ignored) override {
    // |callback_ignored| is dropped because these tests don't exercise multiple
    // channel lifetimes.
    connected_channel_ = channel.Bind();

    if (on_channel_connected_cb_)
      std::move(on_channel_connected_cb_).Run();
=======
    if (cast_channel_.connected_channel())
      return;

    base::RunLoop run_loop;
    cast_channel_.set_on_channel_connected(run_loop.QuitClosure());
    run_loop.Run();

    DCHECK(cast_channel_.connected_channel());
  }

  fuchsia::sys::ComponentControllerPtr StartCastComponent(
      base::StringPiece component_url) {
    DCHECK(!component_services_);

    // Create a ServiceDirectory and publish the CastChannel into it.
    fidl::InterfaceHandle<fuchsia::io::Directory> directory;
    component_services_ = std::make_unique<base::fuchsia::ServiceDirectory>(
        directory.NewRequest());
    cast_channel_binding_ = std::make_unique<
        base::fuchsia::ScopedServiceBinding<chromium::cast::CastChannel>>(
        component_services_.get(), &cast_channel_);

    // Configure the Runner, including a service directory channel to publish
    // services to.
    fuchsia::sys::StartupInfo startup_info;
    startup_info.launch_info.url = component_url.as_string();

    // Place the ServiceDirectory in the |flat_namespace|.
    startup_info.flat_namespace.paths.emplace_back("/svc");
    startup_info.flat_namespace.directories.emplace_back(
        directory.TakeChannel());

    fuchsia::sys::Package package;
    package.resolved_url = component_url.as_string();

    fuchsia::sys::ComponentControllerPtr component_controller;
    cast_runner_ptr_->StartComponent(std::move(package),
                                     std::move(startup_info),
                                     component_controller.NewRequest());

    EXPECT_TRUE(component_controller.is_bound());
    return component_controller;
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  }

 protected:
  const base::RunLoop::ScopedRunTimeoutForTest run_timeout_;
  base::MessageLoopForIO message_loop_;
  net::EmbeddedTestServer test_server_;

  // Returns fake Cast application information to the CastRunner.
  FakeApplicationConfigManager app_config_manager_;
<<<<<<< HEAD
  std::unique_ptr<fidl::Binding<chromium::cast::ApplicationConfigManager>>
      app_config_binding_;

  // The connected Cast Channel.
  chromium::web::MessagePortPtr connected_channel_;

  // A pending on-connect callback, to be invoked once a Cast Channel is
  // received.
  base::OnceClosure on_channel_connected_cb_;

  // Receives connected channels from the CastRunner.
  fidl::Binding<chromium::cast::CastChannel> cast_channel_binding_;

  // ServiceDirectory into which the CastRunner will publish itself.
  std::unique_ptr<base::fuchsia::ServiceDirectory> test_services_;
  std::unique_ptr<base::fuchsia::ServiceDirectoryClient> test_services_client_;
=======
  fidl::Binding<chromium::cast::ApplicationConfigManager> app_config_binding_;

  // Incoming service directory, CastChannel and service bindings.
  std::unique_ptr<base::fuchsia::ServiceDirectory> component_services_;
  FakeCastChannel cast_channel_;
  std::unique_ptr<
      base::fuchsia::ScopedServiceBinding<chromium::cast::CastChannel>>
      cast_channel_binding_;

  // ServiceDirectory into which the CastRunner will publish itself.
  std::unique_ptr<base::fuchsia::ServiceDirectory> public_services_;
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3

  std::unique_ptr<CastRunner> cast_runner_;
  fuchsia::sys::RunnerPtr cast_runner_ptr_;
  base::RunLoop cast_runner_run_loop_;

  DISALLOW_COPY_AND_ASSIGN(CastRunnerIntegrationTest);
};

// A basic integration test ensuring a basic cast request launches the right
// URL in the Chromium service.
TEST_F(CastRunnerIntegrationTest, BasicRequest) {
  const char kBlankAppId[] = "00000000";
  const char kBlankAppPath[] = "/defaultresponse";
  app_config_manager_.AddAppMapping(kBlankAppId,
                                    test_server_.GetURL(kBlankAppPath));

  // Launch the test-app component.
<<<<<<< HEAD
  fuchsia::sys::ComponentControllerPtr component_controller_ptr;
  base::fuchsia::ServiceDirectoryClient services_client(StartCastComponent(
      base::StringPrintf("cast:%s", kBlankAppId), &cast_runner_ptr_,
      component_controller_ptr.NewRequest(), &cast_channel_binding_));
  component_controller_ptr.set_error_handler(&ComponentErrorHandler);
=======
  fuchsia::sys::ComponentControllerPtr component_controller =
      StartCastComponent(base::StringPrintf("cast:%s", kBlankAppId));
  component_controller.set_error_handler(&ComponentErrorHandler);
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3

  // Access the NavigationController from the WebComponent. The test will hang
  // here if no WebComponent was created.
  chromium::web::NavigationControllerPtr nav_controller;
  {
    base::RunLoop run_loop;
    cr_fuchsia::ResultReceiver<WebComponent*> web_component(
        run_loop.QuitClosure());
    cast_runner_->GetWebComponentForTest(web_component.GetReceiveCallback());
    run_loop.Run();
    ASSERT_NE(*web_component, nullptr);
    (*web_component)
        ->frame()
        ->GetNavigationController(nav_controller.NewRequest());
  }

  // Ensure the NavigationEntry has the expected URL.
  {
    base::RunLoop run_loop;
    cr_fuchsia::ResultReceiver<std::unique_ptr<chromium::web::NavigationEntry>>
        nav_entry(run_loop.QuitClosure());
    nav_controller->GetVisibleEntry(
        cr_fuchsia::CallbackToFitFunction(nav_entry.GetReceiveCallback()));
    run_loop.Run();
    EXPECT_EQ(nav_entry->get()->url, test_server_.GetURL(kBlankAppPath).spec());
  }

<<<<<<< HEAD
  // Verify that the component is torn down when |component_controller_ptr| is
=======
  // Verify that the component is torn down when |component_controller| is
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  // unbound, by observing the destruction of its child Interfaces.
  base::RunLoop destruction_run_loop;
  nav_controller.set_error_handler(
      [&destruction_run_loop](zx_status_t) { destruction_run_loop.Quit(); });
<<<<<<< HEAD
  component_controller_ptr.Unbind();
=======
  component_controller.Unbind();
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  destruction_run_loop.Run();
}

TEST_F(CastRunnerIntegrationTest, IncorrectCastAppId) {
<<<<<<< HEAD
  // Launch the test-app component.
  fuchsia::sys::ComponentControllerPtr component_controller_ptr;
  base::fuchsia::ServiceDirectoryClient services_client(StartCastComponent(
      "cast:99999999", &cast_runner_ptr_, component_controller_ptr.NewRequest(),
      &cast_channel_binding_));
  component_controller_ptr.set_error_handler(&ComponentErrorHandler);

  // Ensure no WebComponent was created.
  base::RunLoop run_loop;
=======
  // Launch the a component with an invalid Cast app Id.
  fuchsia::sys::ComponentControllerPtr component_controller =
      StartCastComponent("cast:99999999");

  // Run the loop until the ComponentController is dropped, or a WebComponent is
  // created.
  base::RunLoop run_loop;
  component_controller.set_error_handler([&run_loop](zx_status_t status) {
    EXPECT_EQ(status, ZX_ERR_PEER_CLOSED);
    run_loop.Quit();
  });
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  cr_fuchsia::ResultReceiver<WebComponent*> web_component(
      run_loop.QuitClosure());
  cast_runner_->GetWebComponentForTest(web_component.GetReceiveCallback());
  run_loop.Run();
<<<<<<< HEAD
  EXPECT_EQ(*web_component, nullptr);
=======
  EXPECT_FALSE(web_component.has_value());
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
}

TEST_F(CastRunnerIntegrationTest, CastChannel) {
  const char kCastChannelAppId[] = "00000001";
  const char kCastChannelAppPath[] = "/cast_channel.html";
  app_config_manager_.AddAppMapping(kCastChannelAppId,
                                    test_server_.GetURL(kCastChannelAppPath));

  // Launch the test-app component.
<<<<<<< HEAD
  fuchsia::sys::ComponentControllerPtr component_controller_ptr;
  base::fuchsia::ServiceDirectoryClient services_client(StartCastComponent(
      base::StringPrintf("cast:%s", kCastChannelAppId), &cast_runner_ptr_,
      component_controller_ptr.NewRequest(), &cast_channel_binding_));
  component_controller_ptr.set_error_handler(&ComponentErrorHandler);
=======
  fuchsia::sys::ComponentControllerPtr component_controller =
      StartCastComponent(base::StringPrintf("cast:%s", kCastChannelAppId));
  component_controller.set_error_handler(&ComponentErrorHandler);
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3

  // Access the NavigationController from the WebComponent. The test will hang
  // here if no WebComponent was created.
  chromium::web::NavigationControllerPtr nav_controller;
  {
    base::RunLoop run_loop;
    cr_fuchsia::ResultReceiver<WebComponent*> web_component(
        run_loop.QuitClosure());
    cast_runner_->GetWebComponentForTest(web_component.GetReceiveCallback());
    run_loop.Run();
    ASSERT_NE(*web_component, nullptr);
    (*web_component)
        ->frame()
        ->GetNavigationController(nav_controller.NewRequest());
  }

  // Ensure the NavigationEntry has the expected URL.
  {
    base::RunLoop run_loop;
    cr_fuchsia::ResultReceiver<std::unique_ptr<chromium::web::NavigationEntry>>
        nav_entry(run_loop.QuitClosure());
    nav_controller->GetVisibleEntry(
        cr_fuchsia::CallbackToFitFunction(nav_entry.GetReceiveCallback()));
    run_loop.Run();
    EXPECT_EQ(nav_entry->get()->url,
              test_server_.GetURL(kCastChannelAppPath).spec());
  }

  WaitUntilCastChannelOpened();

  auto expected_list = {"this", "is", "a", "test"};
  for (const std::string& expected : expected_list) {
    base::RunLoop run_loop;
    cr_fuchsia::ResultReceiver<chromium::web::WebMessage> message(
        run_loop.QuitClosure());
<<<<<<< HEAD
    connected_channel_->ReceiveMessage(
=======
    cast_channel_.connected_channel()->ReceiveMessage(
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
        cr_fuchsia::CallbackToFitFunction(message.GetReceiveCallback()));
    run_loop.Run();

    EXPECT_EQ(cr_fuchsia::test::StringFromMemBufferOrDie(message->data),
              expected);
  }

<<<<<<< HEAD
  component_controller_ptr.Unbind();
  base::RunLoop run_loop;
  cast_channel_binding_.set_error_handler(
      [&run_loop](zx_status_t) { run_loop.Quit(); });
=======
  // Shutdown the component and wait for the CastChannel to close.
  component_controller.Unbind();
  base::RunLoop run_loop;
  cast_channel_binding_->SetOnLastClientCallback(run_loop.QuitClosure());
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  run_loop.Run();
}

TEST_F(CastRunnerIntegrationTest, CastChannelConsumerDropped) {
  const char kCastChannelAppId[] = "00000001";
  const char kCastChannelAppPath[] = "/cast_channel.html";
  app_config_manager_.AddAppMapping(kCastChannelAppId,
                                    test_server_.GetURL(kCastChannelAppPath));

  // Launch the test-app component.
<<<<<<< HEAD
  fuchsia::sys::ComponentControllerPtr component_controller_ptr;
  base::fuchsia::ServiceDirectoryClient services_client(StartCastComponent(
      base::StringPrintf("cast:%s", kCastChannelAppId), &cast_runner_ptr_,
      component_controller_ptr.NewRequest(), &cast_channel_binding_));

  // Expect that disconnecting the Cast Channel consumer service will trigger
  // the destruction of the Cast Component.
  cast_channel_binding_.Unbind();
  base::RunLoop run_loop;
  component_controller_ptr.set_error_handler(
      [&run_loop](zx_status_t) { run_loop.Quit(); });
  run_loop.Run();

  EXPECT_FALSE(component_controller_ptr.is_bound());
=======
  fuchsia::sys::ComponentControllerPtr component_controller =
      StartCastComponent(base::StringPrintf("cast:%s", kCastChannelAppId));

  // Expect that disconnecting the Cast Channel consumer service will trigger
  // the destruction of the Cast Component.
  cast_channel_binding_.reset();
  base::RunLoop run_loop;
  component_controller.set_error_handler(
      [&run_loop](zx_status_t) { run_loop.Quit(); });
  run_loop.Run();

  EXPECT_FALSE(component_controller.is_bound());
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
}

TEST_F(CastRunnerIntegrationTest, CastChannelComponentControllerDropped) {
  const char kCastChannelAppId[] = "00000001";
  const char kCastChannelAppPath[] = "/cast_channel.html";
  app_config_manager_.AddAppMapping(kCastChannelAppId,
                                    test_server_.GetURL(kCastChannelAppPath));

  // Launch the test-app component.
<<<<<<< HEAD
  fuchsia::sys::ComponentControllerPtr component_controller_ptr;
  base::fuchsia::ServiceDirectoryClient services_client(StartCastComponent(
      base::StringPrintf("cast:%s", kCastChannelAppId), &cast_runner_ptr_,
      component_controller_ptr.NewRequest(), &cast_channel_binding_));
=======
  fuchsia::sys::ComponentControllerPtr component_controller =
      StartCastComponent(base::StringPrintf("cast:%s", kCastChannelAppId));
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3

  // Expect that disconnecting the ComponentController will kill the Cast
  // Component, which we verify indirectly by listening for the disconnection
  // of one of its CastChannel FIDL client.
<<<<<<< HEAD
  component_controller_ptr.Unbind();
  base::RunLoop run_loop;
  cast_channel_binding_.set_error_handler(
      [&run_loop](zx_status_t) { run_loop.Quit(); });
=======
  component_controller.Unbind();
  base::RunLoop run_loop;
  cast_channel_binding_->SetOnLastClientCallback(run_loop.QuitClosure());
>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
  run_loop.Run();
}

}  // namespace castrunner
