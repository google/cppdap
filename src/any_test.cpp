// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dap/any.h"
#include "dap/typeof.h"
#include "dap/types.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace dap {

struct AnyTestObject {
  dap::integer i;
  dap::number n;
};

DAP_STRUCT_TYPEINFO(AnyTestObject,
                    "AnyTestObject",
                    DAP_FIELD(i, "i"),
                    DAP_FIELD(n, "n"));

}  // namespace dap

TEST(Any, EmptyConstruct) {
  dap::any any;
  ASSERT_TRUE(any.is<dap::null>());
  ASSERT_FALSE(any.is<dap::boolean>());
  ASSERT_FALSE(any.is<dap::integer>());
  ASSERT_FALSE(any.is<dap::number>());
  ASSERT_FALSE(any.is<dap::object>());
  ASSERT_FALSE(any.is<dap::array<dap::integer>>());
  ASSERT_FALSE(any.is<dap::AnyTestObject>());
}

TEST(Any, Boolean) {
  dap::any any(dap::boolean(true));
  ASSERT_TRUE(any.is<dap::boolean>());
  ASSERT_EQ(any.get<dap::boolean>(), dap::boolean(true));
}

TEST(Any, Integer) {
  dap::any any(dap::integer(10));
  ASSERT_TRUE(any.is<dap::integer>());
  ASSERT_EQ(any.get<dap::integer>(), dap::integer(10));
}

TEST(Any, Number) {
  dap::any any(dap::number(123.0f));
  ASSERT_TRUE(any.is<dap::number>());
  ASSERT_EQ(any.get<dap::number>(), dap::number(123.0f));
}

TEST(Any, Array) {
  using array = dap::array<dap::integer>;
  dap::any any(array({10, 20, 30}));
  ASSERT_TRUE(any.is<array>());
  ASSERT_EQ(any.get<array>(), array({10, 20, 30}));
}

TEST(Any, Object) {
  dap::object o;
  o["one"] = dap::integer(1);
  o["two"] = dap::integer(2);
  o["three"] = dap::integer(3);
  dap::any any(o);
  ASSERT_TRUE(any.is<dap::object>());
  if (any.is<dap::object>()) {
    auto got = any.get<dap::object>();
    ASSERT_EQ(got.size(), 3U);
    ASSERT_EQ(got.count("one"), 1U);
    ASSERT_EQ(got.count("two"), 1U);
    ASSERT_EQ(got.count("three"), 1U);
    ASSERT_TRUE(got["one"].is<dap::integer>());
    ASSERT_TRUE(got["two"].is<dap::integer>());
    ASSERT_TRUE(got["three"].is<dap::integer>());
    ASSERT_EQ(got["one"].get<dap::integer>(), dap::integer(1));
    ASSERT_EQ(got["two"].get<dap::integer>(), dap::integer(2));
    ASSERT_EQ(got["three"].get<dap::integer>(), dap::integer(3));
  }
}

TEST(Any, TestObject) {
  dap::any any(dap::AnyTestObject{5, 3.0});
  ASSERT_TRUE(any.is<dap::AnyTestObject>());
  ASSERT_EQ(any.get<dap::AnyTestObject>().i, 5);
  ASSERT_EQ(any.get<dap::AnyTestObject>().n, 3.0);
}

TEST(Any, Assign) {
  dap::any any;
  any = dap::integer(10);
  ASSERT_TRUE(any.is<dap::integer>());
  ASSERT_FALSE(any.is<dap::boolean>());
  ASSERT_FALSE(any.is<dap::AnyTestObject>());
  ASSERT_EQ(any.get<dap::integer>(), dap::integer(10));
  any = dap::boolean(true);
  ASSERT_FALSE(any.is<dap::integer>());
  ASSERT_TRUE(any.is<dap::boolean>());
  ASSERT_FALSE(any.is<dap::AnyTestObject>());
  ASSERT_EQ(any.get<dap::boolean>(), dap::boolean(true));
  any = dap::AnyTestObject{5, 3.0};
  ASSERT_FALSE(any.is<dap::integer>());
  ASSERT_FALSE(any.is<dap::boolean>());
  ASSERT_TRUE(any.is<dap::AnyTestObject>());
  ASSERT_EQ(any.get<dap::AnyTestObject>().i, 5);
  ASSERT_EQ(any.get<dap::AnyTestObject>().n, 3.0);
}

TEST(Any, Reset) {
  dap::any any(dap::integer(10));
  ASSERT_TRUE(any.is<dap::integer>());
  any.reset();
  ASSERT_FALSE(any.is<dap::integer>());
}
