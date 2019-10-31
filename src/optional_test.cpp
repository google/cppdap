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

#include "dap/optional.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

TEST(Optional, EmptyConstruct) {
  dap::optional<int> opt;
  ASSERT_FALSE(opt);
  ASSERT_FALSE(opt.has_value());
}

TEST(Optional, ValueConstruct) {
  dap::optional<int> opt(0);
  ASSERT_TRUE(opt);
  ASSERT_TRUE(opt.has_value());
}

TEST(Optional, CopyConstruct) {
  dap::optional<int> a(10);
  dap::optional<int> b(a);
  ASSERT_EQ(a, b);
  ASSERT_EQ(b.value(), 10);
}

TEST(Optional, CopyCastConstruct) {
  dap::optional<int> a(10);
  dap::optional<uint16_t> b(a);
  ASSERT_EQ(a, b);
  ASSERT_EQ(b.value(), (uint16_t)10);
}

TEST(Optional, MoveConstruct) {
  dap::optional<int> a(10);
  dap::optional<int> b(std::move(a));
  ASSERT_EQ(b.value(), 10);
}

TEST(Optional, MoveCastConstruct) {
  dap::optional<int> a(10);
  dap::optional<uint16_t> b(std::move(a));
  ASSERT_EQ(b.value(), (uint16_t)10);
}

TEST(Optional, AssignValue) {
  dap::optional<int> a;
  a = 10;
  ASSERT_EQ(a.value(), 10);
}

TEST(Optional, AssignOptional) {
  dap::optional<int> a;
  dap::optional<int> b(10);
  a = b;
  ASSERT_EQ(a.value(), 10);
}

TEST(Optional, MoveAssignOptional) {
  dap::optional<int> a;
  dap::optional<int> b(10);
  a = std::move(b);
  ASSERT_EQ(a.value(), 10);
}

TEST(Optional, StarDeref) {
  dap::optional<int> a(10);
  ASSERT_EQ(*a, 10);
}

TEST(Optional, StarDerefConst) {
  const dap::optional<int> a(10);
  ASSERT_EQ(*a, 10);
}

TEST(Optional, ArrowDeref) {
  struct S {
    int i;
  };
  dap::optional<S> a(S{10});
  ASSERT_EQ(a->i, 10);
}

TEST(Optional, ArrowDerefConst) {
  struct S {
    int i;
  };
  const dap::optional<S> a(S{10});
  ASSERT_EQ(a->i, 10);
}

TEST(Optional, Value) {
  const dap::optional<int> a(10);
  ASSERT_EQ(a.value(), 10);
}

TEST(Optional, ValueDefault) {
  const dap::optional<int> a;
  const dap::optional<int> b(20);
  ASSERT_EQ(a.value(10), 10);
  ASSERT_EQ(b.value(10), 20);
}

TEST(Optional, CompareLT) {
  ASSERT_FALSE(dap::optional<int>(5) < dap::optional<int>(3));
  ASSERT_FALSE(dap::optional<int>(5) < dap::optional<int>(5));
  ASSERT_TRUE(dap::optional<int>(5) < dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() < dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() < dap::optional<int>());
}

TEST(Optional, CompareLE) {
  ASSERT_FALSE(dap::optional<int>(5) <= dap::optional<int>(3));
  ASSERT_TRUE(dap::optional<int>(5) <= dap::optional<int>(5));
  ASSERT_TRUE(dap::optional<int>(5) <= dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() <= dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() <= dap::optional<int>());
}

TEST(Optional, CompareGT) {
  ASSERT_TRUE(dap::optional<int>(5) > dap::optional<int>(3));
  ASSERT_FALSE(dap::optional<int>(5) > dap::optional<int>(5));
  ASSERT_FALSE(dap::optional<int>(5) > dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() > dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() > dap::optional<int>());
}

TEST(Optional, CompareGE) {
  ASSERT_TRUE(dap::optional<int>(5) >= dap::optional<int>(3));
  ASSERT_TRUE(dap::optional<int>(5) >= dap::optional<int>(5));
  ASSERT_FALSE(dap::optional<int>(5) >= dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() >= dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() >= dap::optional<int>());
}

TEST(Optional, CompareEQ) {
  ASSERT_FALSE(dap::optional<int>(5) == dap::optional<int>(3));
  ASSERT_TRUE(dap::optional<int>(5) == dap::optional<int>(5));
  ASSERT_FALSE(dap::optional<int>(5) == dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() == dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() == dap::optional<int>());
}

TEST(Optional, CompareNEQ) {
  ASSERT_TRUE(dap::optional<int>(5) != dap::optional<int>(3));
  ASSERT_FALSE(dap::optional<int>(5) != dap::optional<int>(5));
  ASSERT_TRUE(dap::optional<int>(5) != dap::optional<int>(10));
  ASSERT_TRUE(dap::optional<int>() != dap::optional<int>(10));
  ASSERT_FALSE(dap::optional<int>() != dap::optional<int>());
}
