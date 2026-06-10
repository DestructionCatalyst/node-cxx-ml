{
  "targets": [
    {
      "target_name": "ml",
      "cflags!": [ "-fno-exceptions" ],
#       "cflags_cc!": [ "-fno-exceptions", "-std=c++23" ],
      "cflags_cc": [ "-fexceptions", "-std=c++23" ],
      "sources": [
        "cpp_src/index.cc",
        "cpp_src/ml/math.cc",
        "cpp_src/ml/matrix.cc",
        "cpp_src/ml/MatrixPair.cc",
        "cpp_src/ml/models/OrdinaryLeastSquares.cc",
        "cpp_src/ml/preprocess/TrainTestSplit.cc",
        "cpp_src/ml/preprocess/VariableSelection.cc",
        "cpp_src/ml/metrics/regression.cc",

        "cpp_src/bindings/matrix.cc",
        "cpp_src/bindings/models/OrdinaryLeastSquares.cc",
        "cpp_src/bindings/preprocess/TrainTestSplit.cc",
        "cpp_src/bindings/preprocess/VariableSelection.cc",
        "cpp_src/bindings/metrics/index.cc",
        "cpp_src/bindings/metrics/regression.cc",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "cpp_src"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
    }
  ]
}