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

#include "dap/typeof.h"

namespace {

struct NullTI : public dap::TypeInfo {
  using null = dap::null;
  inline std::string name() const { return "null"; }
  inline size_t size() const { return sizeof(null); }
  inline size_t alignment() const { return alignof(null); }
  inline void construct(void* ptr) const { new (ptr) null(); }
  inline void copyConstruct(void* dst, const void* src) const {
    new (dst) null(*reinterpret_cast<const null*>(src));
  }
  inline void destruct(void* ptr) const {
    reinterpret_cast<null*>(ptr)->~null();
  }
  inline bool deserialize(const dap::Deserializer*, void*) const {
    return true;
  }
  inline bool serialize(dap::Serializer*, const void*) const { return true; }
};

static dap::BasicTypeInfo<dap::boolean> booleanTI("boolean");
static dap::BasicTypeInfo<dap::string> stringTI("string");
static dap::BasicTypeInfo<dap::integer> integerTI("integer");
static dap::BasicTypeInfo<dap::number> numberTI("number");
static dap::BasicTypeInfo<dap::object> objectTI("object");
static dap::BasicTypeInfo<dap::any> anyTI("any");
static NullTI nullTI;

}  // namespace

namespace dap {

const TypeInfo* TypeOf<boolean>::type() {
  return &booleanTI;
}
const TypeInfo* TypeOf<string>::type() {
  return &stringTI;
}
const TypeInfo* TypeOf<integer>::type() {
  return &integerTI;
}
const TypeInfo* TypeOf<number>::type() {
  return &numberTI;
}
const TypeInfo* TypeOf<object>::type() {
  return &objectTI;
}
const TypeInfo* TypeOf<any>::type() {
  return &anyTI;
}
const TypeInfo* TypeOf<null>::type() {
  return &nullTI;
}

}  // namespace dap
