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

#ifndef dap_typeof_h
#define dap_typeof_h

#include "typeinfo.h"
#include "types.h"

#include "serialization.h"

namespace dap {

// BasicTypeInfo is an implementation of the TypeInfo interface for the simple
// template type T.
template <typename T>
struct BasicTypeInfo : public TypeInfo {
  BasicTypeInfo(const std::string& name) : name_(name) {}

  // TypeInfo compliance
  inline std::string name() const { return name_; }
  inline size_t size() const { return sizeof(T); }
  inline size_t alignment() const { return alignof(T); }
  inline void construct(void* ptr) const { new (ptr) T(); }
  inline void copyConstruct(void* dst, const void* src) const {
    new (dst) T(*reinterpret_cast<const T*>(src));
  }
  inline void destruct(void* ptr) const { reinterpret_cast<T*>(ptr)->~T(); }
  inline bool deserialize(const Deserializer* d, void* ptr) const {
    return d->deserialize(reinterpret_cast<T*>(ptr));
  }
  inline bool serialize(Serializer* s, const void* ptr) const {
    return s->serialize(*reinterpret_cast<const T*>(ptr));
  }

 private:
  std::string name_;
};

// TypeOf has a template specialization for each DAP type, each declaring a
// const TypeInfo* type() static member function that describes type T.
template <typename T>
struct TypeOf {};

template <>
struct TypeOf<boolean> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<string> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<integer> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<number> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<object> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<any> {
  static const TypeInfo* type();
};

template <>
struct TypeOf<null> {
  static const TypeInfo* type();
};

template <typename T>
struct TypeOf<array<T>> {
  static inline const TypeInfo* type() {
    static BasicTypeInfo<array<T>> typeinfo("array<" +
                                            TypeOf<T>::type()->name() + ">");
    return &typeinfo;
  }
};

template <typename T0, typename... Types>
struct TypeOf<variant<T0, Types...>> {
  static inline const TypeInfo* type() {
    static BasicTypeInfo<variant<T0, Types...>> typeinfo("variant");
    return &typeinfo;
  }
};

template <typename T>
struct TypeOf<optional<T>> {
  static inline const TypeInfo* type() {
    static BasicTypeInfo<optional<T>> typeinfo("optional<" +
                                               TypeOf<T>::type()->name() + ">");
    return &typeinfo;
  }
};

// DAP_OFFSETOF() macro is a generalization of the offsetof() macro defined in
// <cstddef>. It evaluates to the offset of the given field, with fewer
// restrictions than offsetof(). We cast the address '32' and subtract it again,
// because null-dereference is undefined behavior.
#define DAP_OFFSETOF(s, m) \
  ((int)(size_t) & reinterpret_cast<const volatile char&>((((s*)32)->m)) - 32)

// internal functionality
namespace detail {
template <class T, class M>
M member_type(M T::*);
}  // namespace detail

// DAP_TYPEOF() returns the type of the struct (s) member (m).
#define DAP_TYPEOF(s, m) decltype(detail::member_type(&s::m))

// DAP_FIELD() declares a structure field for the DAP_IMPLEMENT_STRUCT_TYPEINFO
// macro.
// FIELD is the name of the struct field.
// NAME is the serialized name of the field, as described by the DAP
// specification.
#define DAP_FIELD(FIELD, NAME)                       \
  dap::Field {                                       \
    NAME, DAP_OFFSETOF(StructTy, FIELD),             \
        TypeOf<DAP_TYPEOF(StructTy, FIELD)>::type(), \
  }

// DAP_DECLARE_STRUCT_TYPEINFO() declares a TypeOf<> specialization for STRUCT.
#define DAP_DECLARE_STRUCT_TYPEINFO(STRUCT)                \
  template <>                                              \
  struct TypeOf<STRUCT> {                                  \
    static constexpr bool has_custom_serialization = true; \
    static const TypeInfo* type();                         \
  }

// DAP_DECLARE_STRUCT_TYPEINFO() implements the type() member function for the
// TypeOf<> specialization for STRUCT.
// STRUCT is the structure typename.
// NAME is the serialized name of the structure, as described by the DAP
// specification. The variadic (...) parameters should be a repeated list of
// DAP_FIELD()s, one for each field of the struct.
#define DAP_IMPLEMENT_STRUCT_TYPEINFO(STRUCT, NAME, ...)                  \
  const TypeInfo* TypeOf<STRUCT>::type() {                                \
    using StructTy = STRUCT;                                              \
    struct TI : BasicTypeInfo<StructTy> {                                 \
      TI() : BasicTypeInfo<StructTy>(NAME) {}                             \
      bool deserialize(const Deserializer* d, void* ptr) const override { \
        return d->deserialize(ptr, {__VA_ARGS__});                        \
      }                                                                   \
      bool serialize(Serializer* s, const void* ptr) const override {     \
        return s->fields(ptr, {__VA_ARGS__});                             \
      }                                                                   \
    };                                                                    \
    static TI typeinfo;                                                   \
    return &typeinfo;                                                     \
  }

// DAP_STRUCT_TYPEINFO() is a helper for declaring and implementing a TypeOf<>
// specialization for STRUCT in a single statement.
#define DAP_STRUCT_TYPEINFO(STRUCT, NAME, ...) \
  DAP_DECLARE_STRUCT_TYPEINFO(STRUCT);         \
  DAP_IMPLEMENT_STRUCT_TYPEINFO(STRUCT, NAME, __VA_ARGS__)

}  // namespace dap

#endif  // dap_typeof_h
