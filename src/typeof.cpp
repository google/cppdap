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

namespace dap {

const TypeInfo* TypeOf<boolean>::type() {
  static BasicTypeInfo<boolean> typeinfo("boolean");
  return &typeinfo;
}

const TypeInfo* TypeOf<string>::type() {
  static BasicTypeInfo<string> typeinfo("string");
  return &typeinfo;
}

const TypeInfo* TypeOf<integer>::type() {
  static BasicTypeInfo<integer> typeinfo("integer");
  return &typeinfo;
}

const TypeInfo* TypeOf<number>::type() {
  static BasicTypeInfo<number> typeinfo("number");
  return &typeinfo;
}

const TypeInfo* TypeOf<object>::type() {
  static BasicTypeInfo<object> typeinfo("object");
  return &typeinfo;
}

const TypeInfo* TypeOf<any>::type() {
  static BasicTypeInfo<any> typeinfo("any");
  return &typeinfo;
}

const TypeInfo* TypeOf<null>::type() {
  struct TI : public TypeInfo {
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
    inline bool deserialize(const Deserializer*, void*) const { return true; }
    inline bool serialize(Serializer*, const void*) const { return true; }
  };
  static TI typeinfo;
  return &typeinfo;
}  // namespace dap

}  // namespace dap
