/*
 * Copyright (C) 2020 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string>
#include <vector>

#include <android-base/strings.h>
#include <vintf/fcm_exclude.h>

namespace android::vintf::details {

// The predicate to VintfObject::checkMissingHalsInMatrices.
bool ShouldCheckMissingHalsInFcm(const std::string& package) {
    using std::placeholders::_1;

    static std::vector<std::string> included_prefixes{
            // Other AOSP HALs (e.g. android.frameworks.*) are not added because only framework
            // matrix is checked.
            "android.hardware.",
    };

    static std::vector<std::string> excluded_prefixes{
            // TODO(b/110261831): reduce items in this list
            "android.hardware.gnss.measurement_corrections@",
            "android.hardware.graphics.bufferqueue@",

            // Exempted.
            "android.hardware.camera.device@",
            "android.hardware.tests.",
    };

    static std::vector<std::string> excluded_exact{
            // TODO(b/110261831): reduce items in this list
            "android.hardware.audio@7.0",
            "android.hardware.audio.effect@7.0",
            "android.hardware.biometrics.fingerprint@2.3",
            "android.hardware.cas.native@1.0",
            "android.hardware.fastboot@1.0",
            "android.hardware.gnss.visibility_control@1.0",
            "android.hardware.media.bufferpool@1.0",
            "android.hardware.media.bufferpool@2.0",
            "android.hardware.radio.config@1.2",
            "android.hardware.tv.cec@2.0",
            "android.hardware.tv.tuner@1.0",
            "android.hardware.keymaster",

            // Exempted
            "android.hardware.common",
            "android.hardware.graphics.common",
    };

    auto package_has_prefix = [&](const std::string& prefix) {
        return android::base::StartsWith(package, prefix);
    };

    // Only check packageAndVersions that are in the include list and not in the exclude list.
    if (!std::any_of(included_prefixes.begin(), included_prefixes.end(), package_has_prefix)) {
        return false;
    }

    if (std::find(excluded_exact.begin(), excluded_exact.end(), package) != excluded_exact.end()) {
        return false;
    }

    return !std::any_of(excluded_prefixes.begin(), excluded_prefixes.end(), package_has_prefix);
}

}  // namespace android::vintf::details
