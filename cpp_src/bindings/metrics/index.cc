#include "regression.h"

namespace node {

    Napi::Object MetricsInit(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Object metrics = Napi::Object::New(env);
        RegressionMetricsInit(env, metrics);

        exports.Set("metrics", metrics);

        return exports;
    }
}