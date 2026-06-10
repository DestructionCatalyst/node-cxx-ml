#ifndef BINDINGS_VARIABLE_SELECTION_H
#define BINDINGS_VARIABLE_SELECTION_H

#include <napi.h>

#include "ml/preprocess/VariableSelection.h"
#include "bindings/matrix.h"

namespace node {
    Napi::Object VariableSelectionInit(Napi::Env env, Napi::Object exports);
    Napi::Value AddConstantVariable(const Napi::CallbackInfo& info);
}

#endif