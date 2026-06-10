#ifndef BINDINGS_METRICS_REGRESSION_H
#define BINDINGS_METRICS_REGRESSION_H

#include <napi.h>

#include "regression.h"

namespace node {
    Napi::Object MetricsInit(Napi::Env env, Napi::Object exports);
}

#endif