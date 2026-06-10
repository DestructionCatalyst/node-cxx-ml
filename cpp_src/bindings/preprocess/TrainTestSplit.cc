#include "TrainTestSplit.h"

namespace node {

    Napi::Object TrainTestSplitInit(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        exports.Set("trainTestSplit", Napi::Function::New(env, TrainTestSplit));
        return exports;
    }

    Napi::Value TrainTestSplit(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsObject()) {
            Napi::TypeError::New(env, "Expected a parameter object").ThrowAsJavaScriptException();
        }

        Napi::Object parameterObject = info[0].As<Napi::Object>();

        Matrix* X;
        std::optional<Matrix*> y = std::optional<Matrix*>();
        float testSize;
        std::optional<size_t> seed = std::optional<size_t>();

        if (parameterObject.Has("X")) {
            Napi::Value xJs = parameterObject.Get("X");

            X = Napi::ObjectWrap<Matrix>::Unwrap(xJs.As<Napi::Object>());
        }
        else {
            Napi::TypeError::New(env, "Missing required parameter X").ThrowAsJavaScriptException();
            throw 1;
        }

        if (parameterObject.Has("y")) {
            Napi::Value yJs = parameterObject.Get("y");

            y = std::make_optional(Napi::ObjectWrap<Matrix>::Unwrap(yJs.As<Napi::Object>()));
        }

        if (parameterObject.Has("testSize")) {
            Napi::Value testSizeJs = parameterObject.Get("testSize");

            if (!testSizeJs.IsNumber()) {
                Napi::TypeError::New(env, "testSize must be a number").ThrowAsJavaScriptException();
            }

            Napi::Number testSizeNumber = testSizeJs.As<Napi::Number>();

            testSize = testSizeNumber.FloatValue();

            if (testSize <= 0 || testSize >= 1) {
                Napi::TypeError::New(env, "testSize must be in range (0, 1)").ThrowAsJavaScriptException();
            }
        }
        else {
            testSize = 0.25f;
        }

        if (parameterObject.Has("seed")) {
            Napi::Value seedJs = parameterObject.Get("seed");

            if (!seedJs.IsNumber()) {
                Napi::TypeError::New(env, "seed must be a number").ThrowAsJavaScriptException();
            }

            Napi::Number seedNumber = seedJs.As<Napi::Number>();

            seed = seedNumber.Int64Value();
        }



        std::array<ml::Matrix*, 4> result = ml::TrainTestSplit(
            *X->matrix,
            // TODO в отдельную функцию
            y.transform([](Matrix* m) { return *m->matrix; }),
            testSize,
            seed
        );

        Napi::Object resultObject = Napi::Object::New(env);

        Napi::Object xTrainInstance = Matrix::EmptyWrapper(env);
        Matrix* xTrainMatrix = Napi::ObjectWrap<Matrix>::Unwrap(xTrainInstance);
        xTrainMatrix->matrix = result[0];
        resultObject.Set("xTrain", xTrainInstance);

        Napi::Object xTestInstance = Matrix::EmptyWrapper(env);
        Matrix* xTestMatrix = Napi::ObjectWrap<Matrix>::Unwrap(xTestInstance);
        xTestMatrix->matrix = result[2];
        resultObject.Set("xTest", xTestInstance);

        if (y) {
            Napi::Object yTrainInstance = Matrix::EmptyWrapper(env);
            Matrix* yTrainMatrix = Napi::ObjectWrap<Matrix>::Unwrap(yTrainInstance);
            yTrainMatrix->matrix = result[1];
            resultObject.Set("yTrain", yTrainInstance);

            Napi::Object yTestInstance = Matrix::EmptyWrapper(env);
            Matrix* yTestMatrix = Napi::ObjectWrap<Matrix>::Unwrap(yTestInstance);
            yTestMatrix->matrix = result[3];
            resultObject.Set("yTest", yTestInstance);
        }

        return resultObject;
    }
}
