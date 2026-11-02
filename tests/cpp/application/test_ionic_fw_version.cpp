// Copyright © Advanced Micro Devices, Inc. All rights reserved.
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#include <gtest/gtest.h>

#include "mori/application/transport/rdma/providers/ionic/ionic.hpp"

namespace mori::application {
namespace {

TEST(IonicFwVersion, ParsesBothBuildTagForms) {
  EXPECT_EQ(ParseIonicFwVersion("1.117.5-a-58"), (IonicFwVersion{1, 117, 5, 58}));
  EXPECT_EQ(ParseIonicFwVersion("1.117.5-a58"), (IonicFwVersion{1, 117, 5, 58}));
  EXPECT_EQ(ParseIonicFwVersion("1.117.5-a-190"), (IonicFwVersion{1, 117, 5, 190}));
}

TEST(IonicFwVersion, RejectsAVersionWithoutABuild) {
  for (const char* version : {"", "unknown", "1.117.5", "1.118.0", "1.117"}) {
    EXPECT_FALSE(ParseIonicFwVersion(version).has_value()) << version;
  }
}

TEST(IonicFwVersion, ComparesMajorThenMinorThenPatchThenBuild) {
  const IonicFwVersion fix{1, 117, 5, 190};
  for (const char* version : {"1.117.5-a-190", "1.117.5-a-191", "1.117.6-a-1", "2.0.0-a-0"}) {
    EXPECT_GE(*ParseIonicFwVersion(version), fix) << version;
  }
  for (const char* version : {"1.117.5-a-77", "1.117.5-a-189", "1.117.4-a-500", "1.116.9-a-999"}) {
    EXPECT_LT(*ParseIonicFwVersion(version), fix) << version;
  }
}

TEST(IonicFwVersion, AMissingDeviceHasNoVersion) {
  EXPECT_FALSE(ReadIonicFwVersion("no-such-rdma-device").has_value());
}

}  // namespace
}  // namespace mori::application
