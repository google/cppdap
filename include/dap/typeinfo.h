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

#ifndef dap_typeinfo_h
#define dap_typeinfo_h

#include <functional>
#include <string>

namespace dap {

class any;
class Deserializer;
class Serializer;

// The TypeInfo interface provides basic runtime type information about DAP
// types. TypeInfo is used by the serialization system to encode and decode DAP
// requests, responses, events and structs.
struct TypeInfo {
  virtual std::string name() const = 0;
  virtual size_t size() const = 0;
  virtual size_t alignment() const = 0;
  virtual void construct(void*) const = 0;
  virtual void copyConstruct(void* dst, const void* src) const = 0;
  virtual void destruct(void*) const = 0;
  virtual bool deserialize(const Deserializer*, void*) const = 0;
  virtual bool serialize(Serializer*, const void*) const = 0;
};

}  // namespace dap

#endif  // dap_typeinfo_h
