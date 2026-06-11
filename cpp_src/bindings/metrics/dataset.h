#ifndef BINDINGS_METRICS_DATASET_H
#define BINDINGS_METRICS_DATASET_H

#include <napi.h>

#include "ml/metrics/dataset.h"
#include "bindings/matrix.h"

namespace node {
    Napi::Object DatasetMetricsInit(Napi::Env env, Napi::Object exports);
    Napi::Value Means(const Napi::CallbackInfo& info);
    Napi::Value Covariation(const Napi::CallbackInfo& info);
    Napi::Value Correlation(const Napi::CallbackInfo& info);
}

#endif