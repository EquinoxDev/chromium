// Copyright 2019 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.components.module_installer;

<<<<<<< HEAD
=======
import android.content.Context;

>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
import org.chromium.base.VisibleForTesting;

import java.util.HashSet;
import java.util.Set;

/** Mock ModuleInstaller for use in tests. */
public class ModuleInstaller {
    private static Set<String> sModulesRequestedDeffered = new HashSet<>();

    public static void init() {}
<<<<<<< HEAD
=======

    public static void initActivity(Context context) {}

>>>>>>> 1edcc2f128d290860af09401391ae79df290b5f3
    public static void updateCrashKeys(){};

    public static void install(
            String moduleName, OnModuleInstallFinishedListener onFinishedListener) {}

    public static void installDeferred(String moduleName) {
        sModulesRequestedDeffered.add(moduleName);
    }

    @VisibleForTesting
    public static boolean didRequestDeferred(String moduleName) {
        return sModulesRequestedDeffered.contains(moduleName);
    }

    private ModuleInstaller() {}
}
