#include "OrdinaryLeastSquares.h"

namespace node {
    Napi::FunctionReference OrdinaryLeastSquares::constructor;

    Napi::Object OrdinaryLeastSquares::Init(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Function func = DefineClass(env, "OrdinaryLeastSquares", {
            InstanceMethod("fit", &OrdinaryLeastSquares::Fit),
            InstanceMethod("predict", &OrdinaryLeastSquares::Predict),
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();

        // TODO wrap into objects
        exports.Set("OrdinaryLeastSquares", func);
        exports.Set("OLS", func);
        return exports;
    }

    OrdinaryLeastSquares::OrdinaryLeastSquares(const Napi::CallbackInfo& info)
        : Napi::ObjectWrap<OrdinaryLeastSquares>(info)
    {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        this->model = new ml::OrdinaryLeastSquares();
    }

    Napi::Value OrdinaryLeastSquares::Fit(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 2) {
            Napi::TypeError::New(env, "Expected 2 matrices").ThrowAsJavaScriptException();
        }

        // TODO move to common
        Napi::Object xJs = info[0].As<Napi::Object>();

        // TODO somehow check that it's matrix

        Matrix* X  = Napi::ObjectWrap<Matrix>::Unwrap(xJs);

        Napi::Object yJs = info[1].As<Napi::Object>();

        // TODO somehow check that it's matrix

        Matrix* y = Napi::ObjectWrap<Matrix>::Unwrap(yJs);

        this->model->Fit((*X->matrix), (*y->matrix));

        return env.Undefined();
    }

     Napi::Value OrdinaryLeastSquares::Predict(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1) {
            Napi::TypeError::New(env, "Expected 1 matrix").ThrowAsJavaScriptException();
        }

        // TODO move to common
        Napi::Object xJs = info[0].As<Napi::Object>();

        // TODO somehow check that it's matrix

        Matrix* X = Napi::ObjectWrap<Matrix>::Unwrap(xJs);

        ml::Matrix yPred = this->model->Predict(*X->matrix);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = new ml::Matrix(yPred);

        return instance;
     }
}
