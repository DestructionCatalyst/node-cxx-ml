#ifndef BINDINGS_MATRIX_H
#define BINDINGS_MATRIX_H

#include <napi.h>
#include "ml/matrix.h"

namespace node {
    class Matrix : public Napi::ObjectWrap<Matrix> {
     public:
      static Napi::Object Init(Napi::Env env, Napi::Object exports); //Init function for setting the export key to JS
      Matrix(const Napi::CallbackInfo& info);

      // Factory methods
      static Napi::Value FromArray(const Napi::CallbackInfo& info);
      static Napi::Value Zeros(const Napi::CallbackInfo& info);
      static Napi::Value Identity(const Napi::CallbackInfo& info);

      Napi::Value Add(const Napi::CallbackInfo& info);
      Napi::Value Subtract(const Napi::CallbackInfo& info);
      Napi::Value MultiplyBy(const Napi::CallbackInfo& info);
      Napi::Value Multiply(const Napi::CallbackInfo& info);
      Napi::Value Transpose(const Napi::CallbackInfo& info);
      Napi::Value Inverse(const Napi::CallbackInfo& info);
      Napi::Value Project(const Napi::CallbackInfo& info);

      Napi::Value ToArray(const Napi::CallbackInfo& info);

      static Napi::Object EmptyWrapper(Napi::Env env);
      static Napi::Object Wrap(Napi::Env env, ml::Matrix);
//       static Matrix* FromParams(const Napi::CallbackInfo& info, size_t index);

      ~Matrix();

      ml::Matrix *matrix; //internal instance of matrix used to perform actual operations.

     private:
      static Napi::FunctionReference constructor; //reference to store the class definition that needs to be exported to JS
      Napi::Value GetRowCount(const Napi::CallbackInfo& info); //wrapped getWidth function
      Napi::Value GetColCount(const Napi::CallbackInfo& info); //wrapped getHeight function
      static Matrix* OtherMatrixFromParams(const Napi::CallbackInfo& info);
    };
}

#endif
