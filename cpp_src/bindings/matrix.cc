#include "matrix.h"
#include <iostream>
#include <string>

const std::string EMPTY_WRAPPER = "aBFICsUlt5dMtr1hb0ROb04Y8jkHSd";

namespace node {
    Napi::FunctionReference Matrix::constructor;

    Napi::Object Matrix::Init(Napi::Env env, Napi::Object exports) {
        Napi::HandleScope scope(env);

        Napi::Function func = DefineClass(env, "Matrix", {
            StaticMethod("zeros", &Matrix::Zeros),
            StaticMethod("identity", &Matrix::Identity),
            StaticMethod("fromArray", &Matrix::FromArray),
            InstanceMethod("toArray", &Matrix::ToArray),
            InstanceMethod("add", &Matrix::Add),
            InstanceMethod("subtract", &Matrix::Subtract),
            InstanceMethod("multiplyBy", &Matrix::MultiplyBy),
            InstanceMethod("multiply", &Matrix::Multiply),
            InstanceMethod("transpose", &Matrix::Transpose),
            InstanceMethod("inverse", &Matrix::Inverse),
            InstanceMethod("project", &Matrix::Project),
            InstanceMethod("getRowCount", &Matrix::GetRowCount),
            InstanceMethod("getColCount", &Matrix::GetColCount),
        });

        constructor = Napi::Persistent(func);
        constructor.SuppressDestruct();

        exports.Set("Matrix", func);
        return exports;
    }

    Matrix::Matrix(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Matrix>(info)  {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 2) {
            if (length == 1 && info[0].IsString()) {
                 std::string code = (std::string) info[0].As<Napi::String>();
                 if (code == EMPTY_WRAPPER) {
                    this->matrix = nullptr;
                    return;
                 }
                 else {
                     Napi::TypeError::New(env, "Expected 2 positive integers").ThrowAsJavaScriptException();
                 }
            }
            else {
                Napi::TypeError::New(env, "Expected 2 positive integers").ThrowAsJavaScriptException();
            }
        }

        bool isNotNumber = !info[0].IsNumber() || !info[1].IsNumber();
        Napi::Number nRowsJs = info[0].As<Napi::Number>();
        Napi::Number nColsJs = info[1].As<Napi::Number>();

        size_t nRows = nRowsJs.Uint32Value();
        size_t nCols = nColsJs.Uint32Value();

        bool isNotPositive = nRows <= 0 || nCols <= 0;

        if (isNotNumber || isNotPositive) {
            Napi::TypeError::New(env, "Expected 2 positive integers").ThrowAsJavaScriptException();
        }

        ml::Matrix* mlMatrix = ml::Matrix::Zeros(nRows, nCols);

        this->matrix = mlMatrix;
    }

    Napi::Object Matrix::EmptyWrapper(Napi::Env env) {
        std::vector<napi_value> args;

        napi_value emptyWrapperNapi;
        napi_create_string_utf8(env, EMPTY_WRAPPER.c_str(), EMPTY_WRAPPER.length(), &emptyWrapperNapi);
        args.push_back(emptyWrapperNapi);

        Napi::Object instance = constructor.Value().New(args);

        return instance;
    }

    Napi::Value Matrix::Zeros(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 2) {
            Napi::TypeError::New(env, "Expected 2 positive integers").ThrowAsJavaScriptException();
        }

        std::vector<napi_value> args;

        args.push_back(info[0]);
        args.push_back(info[1]);

        Napi::Object instance = constructor.Value().New(args);

        return instance;
    }

    Napi::Value Matrix::Identity(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsNumber()) {
            Napi::TypeError::New(env, "Expected 1 positive integer").ThrowAsJavaScriptException();
        }

        Napi::Number dimJs = info[0].As<Napi::Number>();

        size_t dim = dimJs.Uint32Value();

        bool isNotPositive = dim <= 0;

        if (isNotPositive) {
            Napi::TypeError::New(env, "Expected 1 positive integer").ThrowAsJavaScriptException();
        }

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        ml::Matrix* mlMatrix = ml::Matrix::Identity(dim);

        result->matrix = mlMatrix;

        return instance;
    }

    Napi::Value Matrix::FromArray(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsArray()) {
            Napi::TypeError::New(env, "Expected 1 array of arrays with same length").ThrowAsJavaScriptException();
        }

        Napi::Array matrixJs = info[0].As<Napi::Array>();

        if (matrixJs.Length() < 1 || !(matrixJs.Get(0u).IsArray())) {
            Napi::TypeError::New(env, "Expected 1 array of arrays with same length").ThrowAsJavaScriptException();
        }

        Napi::Array firstRow = matrixJs.Get(0u).As<Napi::Array>();

        size_t nRows = matrixJs.Length();
        size_t nCols = firstRow.Length();

        std::vector<napi_value> args;
        napi_value nRowsNapi;
        napi_create_uint32(env, nRows, &nRowsNapi);
        napi_value nColsNapi;
        napi_create_uint32(env, nCols, &nColsNapi);
        args.push_back(nRowsNapi);
        args.push_back(nColsNapi);

        Napi::Object instance = constructor.Value().New(args);

        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        for (size_t row = 0; row < nRows; row++) {
            Napi::Value currentRowValue = matrixJs.Get(row);

            if (!currentRowValue.IsArray()) {
                Napi::TypeError::New(env, "Expected 1 array of arrays with same length").ThrowAsJavaScriptException();
            }

            Napi::Array currentRow = currentRowValue.As<Napi::Array>();

            if (currentRow.Length() != nCols) {
                Napi::TypeError::New(env, "Expected 1 array of arrays with same length").ThrowAsJavaScriptException();
            }

            for (size_t col = 0; col < nCols; col++) {
                Napi::Value currentValue = currentRow.Get(col);

                if (!currentValue.IsNumber()) {
                    Napi::TypeError::New(env, "Arrays must only contain numbers").ThrowAsJavaScriptException();
                }

                Napi::Number currentNumber = currentValue.As<Napi::Number>();

                (*result->matrix)[row, col] = currentNumber.FloatValue();
            }
        }

        return instance;
    }

    Napi::Value Matrix::ToArray(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Napi::Array result = Napi::Array::New(env, this->matrix->GetRowCount());

        for (size_t row = 0; row < this->matrix->GetRowCount(); row++) {
            Napi::Array currentRow = Napi::Array::New(env, this->matrix->GetColCount());

            for (size_t col = 0; col < this->matrix->GetColCount(); col++) {
                currentRow.Set(col, (*this->matrix)[row, col]);
            }

            result.Set(row, currentRow);
        }

        return result;
    }

    Napi::Object Matrix::Wrap(Napi::Env env, ml::Matrix matrix) {
        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = new ml::Matrix(matrix);

        return instance;
    }

    Matrix* Matrix::OtherMatrixFromParams(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();

        int length = info.Length();

        if (length != 1 || !info[0].IsObject()) {
            Napi::TypeError::New(env, "Expected 1 matrix").ThrowAsJavaScriptException();
        }

        Napi::Object matrixJs = info[0].As<Napi::Object>();

        if (!matrixJs.InstanceOf(constructor.Value())) {
            Napi::TypeError::New(env, "Expected 1 matrix").ThrowAsJavaScriptException();
        }

        return Napi::ObjectWrap<Matrix>::Unwrap(matrixJs);
    }

    Napi::Value Matrix::Add(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Matrix* other = Matrix::OtherMatrixFromParams(info);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &((*(this->matrix)) + (*(other->matrix)));

        return instance;
    }

    Napi::Value Matrix::Subtract(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Matrix* other = Matrix::OtherMatrixFromParams(info);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &((*(this->matrix)) - (*(other->matrix)));

        return instance;
    }

    Napi::Value Matrix::MultiplyBy(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsNumber()) {
            Napi::TypeError::New(env, "Expected 1 number").ThrowAsJavaScriptException();
        }

        Napi::Number factorJs = info[0].As<Napi::Number>();

        float factor = factorJs.FloatValue();

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &((*(this->matrix)) * factor);

        return instance;
    }

    Napi::Value Matrix::Multiply(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Matrix* other = Matrix::OtherMatrixFromParams(info);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &((*(this->matrix)) * (*(other->matrix)));

        return instance;
    }

    Napi::Value Matrix::Transpose(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &(this->matrix->Transpose());

        return instance;
    }

    Napi::Value Matrix::Inverse(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &(this->matrix->Inverse());

        return instance;
    }

    Napi::Value Matrix::Project(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        int length = info.Length();

        if (length != 1 || !info[0].IsObject()) {
            Napi::TypeError::New(env, "Expected a parameter object").ThrowAsJavaScriptException();
        }

        Napi::Object parameterObject = info[0].As<Napi::Object>();

        std::optional<std::vector<size_t>> rows = std::optional<std::vector<size_t>>();
        std::optional<std::vector<size_t>> cols = std::optional<std::vector<size_t>>();
        ml::ProjectMode rowsMode = ml::ProjectMode::INCLUDE;
        ml::ProjectMode colsMode = ml::ProjectMode::INCLUDE;

        std::optional<Napi::Value> rowsObject = std::optional<Napi::Value>();
        std::optional<Napi::Value> colsObject = std::optional<Napi::Value>();

        if (parameterObject.Has("rows")) {
            rowsObject = std::make_optional(parameterObject.Get("rows"));
        }
        if (parameterObject.Has("excludeRows")) {
            if (rowsObject) {
                Napi::TypeError::New(env, "Can't have both rows and excludeRows").ThrowAsJavaScriptException();
            }
            rowsMode = ml::ProjectMode::EXCLUDE;
            rowsObject = std::make_optional(parameterObject.Get("excludeRows"));
        }

        if (rowsObject) {
            if (!rowsObject->IsArray()) {
                Napi::TypeError::New(env, "Rows must be specified as an array").ThrowAsJavaScriptException();
            }

            Napi::Array rowsArray = rowsObject->As<Napi::Array>();
            rows = std::make_optional(std::vector<size_t>());

            for (size_t i = 0; i < rowsArray.Length(); i++) {
                Napi::Value rowNumberValue = rowsArray.Get(i);

                if (!rowNumberValue.IsNumber()) {
                    Napi::TypeError::New(env, "Row numbers must be integers").ThrowAsJavaScriptException();
                }

                size_t rowNumber = rowNumberValue.As<Napi::Number>().Uint32Value();

                rows->push_back(rowNumber);
            }
        }

        // TODO cols
        if (parameterObject.Has("cols")) {
            colsObject = std::make_optional(parameterObject.Get("cols"));
        }
        if (parameterObject.Has("excludeCols")) {
            if (colsObject) {
                Napi::TypeError::New(env, "Can't have both cols and excludeCols").ThrowAsJavaScriptException();
            }
            colsMode = ml::ProjectMode::EXCLUDE;
            colsObject = std::make_optional(parameterObject.Get("excludeCols"));
        }

        if (colsObject) {
            if (!colsObject->IsArray()) {
                Napi::TypeError::New(env, "Rows must be specified as an array").ThrowAsJavaScriptException();
            }

            Napi::Array colsArray = colsObject->As<Napi::Array>();
            cols = std::make_optional(std::vector<size_t>());

            for (size_t i = 0; i < colsArray.Length(); i++) {
                Napi::Value colNumberValue = colsArray.Get(i);

                if (!colNumberValue.IsNumber()) {
                    Napi::TypeError::New(env, "Column numbers must be integers").ThrowAsJavaScriptException();
                }

                size_t colNumber = colNumberValue.As<Napi::Number>().Uint32Value();

                cols->push_back(colNumber);
            }
        }

        Napi::Object instance = Matrix::EmptyWrapper(env);
        Matrix* result = Napi::ObjectWrap<Matrix>::Unwrap(instance);

        result->matrix = &(this->matrix->Project(rows, cols, rowsMode, colsMode));

        return instance;
    }

    Napi::Value Matrix::GetRowCount(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        size_t num = this->matrix->GetRowCount();
        return Napi::Number::New(env, num);
    }

    Napi::Value Matrix::GetColCount(const Napi::CallbackInfo& info) {
        Napi::Env env = info.Env();
        Napi::HandleScope scope(env);

        size_t num = this->matrix->GetColCount();
        return Napi::Number::New(env, num);
    }

    Matrix::~Matrix() {
        delete this->matrix;
    }
}
