// RUN: %clang_cc1 -verify -triple x86_64-apple-darwin10 -fopenmp=libiomp5 -x c++ -emit-llvm %s -o - | FileCheck %s
// RUN: %clang_cc1 -fopenmp=libiomp5 -x c++ -triple x86_64-apple-darwin10 -emit-pch -o %t %s
// RUN: %clang_cc1 -fopenmp=libiomp5 -x c++ -triple x86_64-apple-darwin10 -include-pch %t -verify %s -emit-llvm -o - | FileCheck %s
// expected-no-diagnostics

#ifndef HEADER
#define HEADER

void foo() {}

template <class T>
T tmain(T argc) {
  static T a;
#pragma omp taskwait
  return a + argc;
}
int main(int argc, char **argv) {
#pragma omp taskwait
  return tmain(argc);
}

// CHECK-LABEL: @main
// CHECK: [[GTID:%.+]] = call i32 @__kmpc_global_thread_num(%{{.+}}* @{{.+}})
// CHECK: call i32 @__kmpc_omp_taskwait(%{{.+}}* @{{.+}}, i32 [[GTID]])

// CHECK-LABEL: tmain
// CHECK: [[GTID:%.+]] = call i32 @__kmpc_global_thread_num(%{{.+}}* @{{.+}})
// CHECK: call i32 @__kmpc_omp_taskwait(%{{.+}}* @{{.+}}, i32 [[GTID]])


#endif
