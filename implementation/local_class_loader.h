/*
 * Copyright 2021 Google LLC
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

#ifndef JNI_BIND_LOCAL_CLASS_LOADER_H_
#define JNI_BIND_LOCAL_CLASS_LOADER_H_

#include "class_defs/java_lang_classes.h"
#include "implementation/class.h"
#include "implementation/class_loader.h"
#include "implementation/class_loader_ref.h"
#include "implementation/jni_helper/lifecycle_object.h"
#include "implementation/jvm.h"
#include "implementation/jvm_ref.h"
#include "implementation/promotion_mechanics.h"
#include "jni_dep.h"

namespace jni {

template <const auto& class_loader_v_, const auto& jvm_v_ = kDefaultJvm>
class LocalClassLoader
    : public LocalCtor<
          LocalClassLoader<class_loader_v_, jvm_v_>,
          ClassLoaderRef<jvm_v_, class_loader_v_>,
          JniT<jobject, kJavaLangClassLoader, class_loader_v_, jvm_v_>,
          jobject> {
 public:
  using Base =
      LocalCtor<LocalClassLoader<class_loader_v_, jvm_v_>,
                ClassLoaderRef<jvm_v_, class_loader_v_>,
                JniT<jobject, kJavaLangClassLoader, class_loader_v_, jvm_v_>,
                jobject>;
  using Base::Base;

  template <const auto& class_loader_v, const auto& jvm_v>
  LocalClassLoader(LocalClassLoader<class_loader_v, jvm_v>&& rhs)
      : Base(rhs.Release()) {}

  ~LocalClassLoader() {
    if (Base::object_ref_) {
      LifecycleHelper<jobject, LifecycleType::LOCAL>::Delete(Base::object_ref_);
    }
  }

 private:
  template <typename>
  friend class ObjectRef;
};

}  // namespace jni

#endif  // JNI_BIND_LOCAL_CLASS_LOADER_H_
