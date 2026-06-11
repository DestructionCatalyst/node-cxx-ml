{
  "targets": [
    {
      "target_name": "ml",
      "cflags!": [ "-fno-exceptions" ],
      "sources": [
        "cpp_src/index.cc",
        "cpp_src/ml/math.cc",
        "cpp_src/ml/matrix.cc",
        "cpp_src/ml/MatrixPair.cc",
        "cpp_src/ml/models/OrdinaryLeastSquares.cc",
        "cpp_src/ml/preprocess/TrainTestSplit.cc",
        "cpp_src/ml/preprocess/VariableSelection.cc",
        "cpp_src/ml/metrics/regression.cc",
        "cpp_src/ml/metrics/dataset.cc",

        "cpp_src/bindings/matrix.cc",
        "cpp_src/bindings/models/OrdinaryLeastSquares.cc",
        "cpp_src/bindings/preprocess/TrainTestSplit.cc",
        "cpp_src/bindings/preprocess/VariableSelection.cc",
        "cpp_src/bindings/metrics/index.cc",
        "cpp_src/bindings/metrics/regression.cc",
        "cpp_src/bindings/metrics/dataset.cc",
      ],
      "include_dirs": [
        "<!@(node -p \"require('node-addon-api').include\")",
        "cpp_src"
      ],
      'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ],
      "conditions": [
          [ 'OS=="win"', {
            "msvs_settings": {
              "VCCLCompilerTool": {
                "AdditionalOptions": [ "/std:c++23" ],
                "ExceptionHandling": 1
              }
            }
          }],
          [ 'OS=="mac"', {
            "xcode_settings": {
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "CLANG_CXX_LANGUAGE_STANDARD": "c++23"
            }
          }],
          [ 'OS=="linux" or OS=="freebsd" or OS=="openbsd" or OS=="solaris"', {
            "cflags_cc": [ "-fexceptions", "-std=c++23" ],
          }]
      ]
    }
  ]
}