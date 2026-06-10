#ifndef BINDINGS_ORDINARY_LEAST_SQUARES_H
#define BINDINGS_ORDINARY_LEAST_SQUARES_H

#include <napi.h>

#include "ml/models/OrdinaryLeastSquares.h"
#include "bindings/matrix.h"

namespace node {
    class OrdinaryLeastSquares: public Napi::ObjectWrap<OrdinaryLeastSquares> {
      public:
        static Napi::Object Init(Napi::Env env, Napi::Object exports);
        OrdinaryLeastSquares(const Napi::CallbackInfo& info);
        Napi::Value Fit(const Napi::CallbackInfo& info);
        Napi::Value Predict(const Napi::CallbackInfo& info);
      private:
        static Napi::FunctionReference constructor;
        ml::OrdinaryLeastSquares* model;
    };
}

#endif