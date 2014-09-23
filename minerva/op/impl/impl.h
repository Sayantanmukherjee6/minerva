#pragma once
#include "op/context.h"

namespace minerva {

template<typename C>
class FnBundle {
};

#ifdef HAS_CUDA

#define INSTALL_COMPUTE_FN(closure_name, basic_fn, mkl_fn, cuda_fn) \
  template<> class FnBundle<closure_name> {\
   public:\
    static void Call(const DataList& i, const DataList& o, closure_name& c, const Context& context) {\
      switch (context.impl_type) {\
        case ImplType::kBasic: basic_fn(i, o, c); break;\
        case ImplType::kMkl: mkl_fn(i, o, c); break;\
        case ImplType::kCuda: cuda_fn(i, o, c, dynamic_cast<const CudaRuntimeContext&>(context)); break; \
        default: NO_IMPL(i, o, c); break;\
      }\
    }\
  };

#define INSTALL_DATAGEN_FN(closure_name, basic_fn, mkl_fn, cuda_fn) \
  template<> class FnBundle<closure_name> {\
   public:\
    static void Call(const DataList& d, closure_name& c, const Context& context) {\
      switch (context.impl_type) {\
        case ImplType::kBasic: basic_fn(d, c); break;\
        case ImplType::kMkl: mkl_fn(d, c, dynamic_cast<const CudaRuntimeContext&>(context)); break;\
        case ImplType::kCuda: cuda_fn(d, c); break;\
        default: NO_IMPL(d, c); break;\
      }\
    }\
  };

#else

#define INSTALL_COMPUTE_FN(closure_name, basic_fn, mkl_fn, cuda_fn) \
  template<> class FnBundle<closure_name> {\
   public:\
    static void Call(const DataList& i, const DataList& o, closure_name& c, const Context& context) {\
      switch (context.impl_type) {\
        case ImplType::kBasic: basic_fn(i, o, c); break;\
        case ImplType::kMkl: mkl_fn(i, o, c); break;\
        default: NO_IMPL(i, o, c); break;\
      }\
    }\
  };

#define INSTALL_DATAGEN_FN(closure_name, basic_fn, mkl_fn, cuda_fn) \
  template<> class FnBundle<closure_name> {\
   public:\
    static void Call(const DataList& d, closure_name& c, const Context& context) {\
      switch (context.impl_type) {\
        case ImplType::kBasic: basic_fn(d, c); break;\
        case ImplType::kMkl: mkl_fn(d, c, dynamic_cast<const CudaRuntimeContext&>(context)); break;\
        default: NO_IMPL(d, c); break;\
      }\
    }\
  };

#endif

}
