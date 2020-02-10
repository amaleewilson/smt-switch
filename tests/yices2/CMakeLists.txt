set(YICES2_TESTS
      yices2-tests
      yices2-arrays
      yices2-arrays-2
      yices2-data-structures
      yices2-incremental
      yices2-traversal
      yices2-int-arithmetic # does not work, nonlinear arithmetic. 
      yices2-transfer # does not work, no term translator yet
      yices2-substitute  
      yices2-substitute2
      yices2-indexed-op-tests # finish get_op()
      yices2-array-models # Works but Cannot do get val on array because it is a function
      yices2-ext-ops
      yices2-const-arrays # does not work, no term translator yet
      yices2-ite-test
      yices2-neg-numbers
      yices2-polynomial
)

foreach(test ${YICES2_TESTS})
  add_executable(${test}.out ${test}.cpp)
  target_include_directories (${test}.out PUBLIC "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries(${test}.out smt-switch)
  target_link_libraries(${test}.out smt-switch-yices2)
  add_test(${test} ${test}.out)
endforeach()

# add_executable(ygt ygt.cpp)
# target_include_directories (ygt PUBLIC "${PROJECT_SOURCE_DIR}/include")
# target_link_libraries(ygt gtest_main)
# target_link_libraries(ygt smt-switch)
# target_link_libraries(ygt smt-switch-yices2)
# add_test(NAME ygt_test COMMAND ygt)

add_executable(yices2-gtest yices2-gtest.cpp)
target_include_directories (yices2-gtest PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(yices2-gtest gtest_main)
target_link_libraries(yices2-gtest smt-switch)
target_link_libraries(yices2-gtest smt-switch-yices2)
add_test(NAME ygt_test COMMAND yices2-gtest)
