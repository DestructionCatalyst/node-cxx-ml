#include "regression.h"

namespace node {

    Napi::Object RegressionMetricsInit(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Object regressionMetrics = Napi::Object::New(env);

        regressionMetrics.Set("meanSquaredError", Napi::Function::New(env, MeanSquaredError));
        regressionMetrics.Set("meanAbsoluteError", Napi::Function::New(env, MeanAbsoluteError));
        regressionMetrics.Set("rSquared", Napi::Function::New(env, RSquared));
//         regressionMetrics.Set("adjustedRSquared", Napi::Function::New(env, AdjustedRSquared));

        exports.Set("regression", regressionMetrics);

        return exports;
    }

    struct YTruePred {
        Matrix* yTrue;
        Matrix* yPred;
    };

    YTruePred GetVectors(const Napi::CallbackInfo& info) {
         Napi::Env env = info.Env();
          Napi::HandleScope scope(env);

          int length = info.Length();

          if (length != 2) {
              Napi::TypeError::New(env, "Expected 2 vectors").ThrowAsJavaScriptException();
          }

          Napi::Object yTrueJs = info[0].As<Napi::Object>();
          Napi::Object yPredJs = info[1].As<Napi::Object>();

          Matrix* yTrue = Napi::ObjectWrap<Matrix>::Unwrap(yTrueJs);
          Matrix* yPred = Napi::ObjectWrap<Matrix>::Unwrap(yPredJs);

          YTruePred result { yTrue, yPred };

          return result;
    }

    Napi::Value MeanSquaredError(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        YTruePred vectors = GetVectors(info);
        Matrix* yTrue = vectors.yTrue;
        Matrix* yPred = vectors.yPred;

        float result = ml::MeanSquaredError(*yTrue->matrix, *yPred->matrix);

        return Napi::Number::New(env, result);
    }

    Napi::Value MeanAbsoluteError(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        YTruePred vectors = GetVectors(info);
        Matrix* yTrue = vectors.yTrue;
        Matrix* yPred = vectors.yPred;

        float result = ml::MeanAbsoluteError(*yTrue->matrix, *yPred->matrix);

        return Napi::Number::New(env, result);
    }

    Napi::Value RSquared(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        YTruePred vectors = GetVectors(info);
        Matrix* yTrue = vectors.yTrue;
        Matrix* yPred = vectors.yPred;

        float result = ml::RSquared(*yTrue->matrix, *yPred->matrix);

        return Napi::Number::New(env, result);
    }
}
