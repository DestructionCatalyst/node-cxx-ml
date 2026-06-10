#ifndef BINDINGS_TRAIN_TEST_SPLIT_H
#define BINDINGS_TRAIN_TEST_SPLIT_H

#include <napi.h>

#include "ml/preprocess/TrainTestSplit.h"
#include "bindings/matrix.h"

namespace node {
    Napi::Object TrainTestSplitInit(Napi::Env env, Napi::Object exports);
    Napi::Value TrainTestSplit(const Napi::CallbackInfo& info);
}

#endif