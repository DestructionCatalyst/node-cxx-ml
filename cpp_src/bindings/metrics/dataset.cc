#include "dataset.h"

namespace node {

    Napi::Object DatasetMetricsInit(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Object datasetMetrics = Napi::Object::New(env);

        datasetMetrics.Set("means", Napi::Function::New(env, Means));
        datasetMetrics.Set("covariation", Napi::Function::New(env, Covariation));
        datasetMetrics.Set("correlation", Napi::Function::New(env, Correlation));

        exports.Set("dataset", datasetMetrics);

        return exports;
    }

    Napi::Value Means(const Napi::CallbackInfo& info) {
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

        ml::Matrix means = ml::Means(*X->matrix);

        return Matrix::Wrap(env, means);
    }

    Napi::Value Covariation(const Napi::CallbackInfo& info) {
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

        ml::Matrix cov = ml::Covariation(*X->matrix);

        return Matrix::Wrap(env, cov);
    }

    Napi::Value Correlation(const Napi::CallbackInfo& info) {
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

        try {
            ml::Matrix corr = ml::Correlation(*X->matrix);

            return Matrix::Wrap(env, corr);
        }
        catch (std::logic_error exception) {
            Napi::Error::New(env, exception.what()).ThrowAsJavaScriptException();

            return env.Undefined();
        }
    }
}
