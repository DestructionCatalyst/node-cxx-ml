#ifndef BINDINGS_METRICS_REGRESSION_H
#define BINDINGS_METRICS_REGRESSION_H

#include <napi.h>

#include "ml/metrics/regression.h"
#include "bindings/matrix.h"

namespace node {
    Napi::Object RegressionMetricsInit(Napi::Env env, Napi::Object exports);
    Napi::Value MeanSquaredError(const Napi::CallbackInfo& info);
    Napi::Value MeanAbsoluteError(const Napi::CallbackInfo& info);
    Napi::Value RSquared(const Napi::CallbackInfo& info);
    Napi::Value AdjustedRSquared(const Napi::CallbackInfo& info);
}

#endif