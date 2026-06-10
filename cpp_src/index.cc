#include <napi.h>
#include "bindings/matrix.h"
#include "bindings/models/OrdinaryLeastSquares.h"
#include "bindings/preprocess/TrainTestSplit.h"
#include "bindings/preprocess/VariableSelection.h"
#include "bindings/metrics/index.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    node::Matrix::Init(env, exports);
    node::OrdinaryLeastSquares::Init(env, exports);
    node::TrainTestSplitInit(env, exports);
    node::MetricsInit(env, exports);
    node::VariableSelectionInit(env, exports);


//     Napi::Object metrics = node::MetricsInit(env, exports);
//     Napi::Object metrics = Napi::Object::New(env);
//     Napi::Object regression = Napi::Object::New(env);
//     metrics.Set("meanSquaredError", Napi::Function::New(env, node::MeanSquaredError));
//     exports.Set("metrics", metrics);

    return exports;
}

NODE_API_MODULE(hello, Init)