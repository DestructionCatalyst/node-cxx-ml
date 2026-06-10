#include "VariableSelection.h"

namespace node {

    Napi::Object VariableSelectionInit(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        exports.Set("addConstantVariable", Napi::Function::New(env, AddConstantVariable));
        return exports;
    }

    Napi::Value AddConstantVariable(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsObject()) {
            Napi::TypeError::New(env, "Expected a matrix").ThrowAsJavaScriptException();
        }

        Napi::Object xJs = info[0].As<Napi::Object>();

        Matrix* X = Napi::ObjectWrap<Matrix>::Unwrap(xJs);

        ml::Matrix xTransformed = AddConstantVariable(*X->matrix);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = new ml::Matrix(xTransformed);

        return instance;
    }
}
