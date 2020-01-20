set(YICES2_TESTS
      yices2-tests
      yices2-arrays
      yices2-arrays-2
    #  yices2-arrays2
      yices2-data-structures
      yices2-incremental
    #  yices2-traversal
      yices2-int-arithmetic
    #  yices2-transfer
    #  yices2-substitute
    #  yices2-indexed-op-tests
    #  yices2-array-models
    #  yices2-ext-ops
    #  yices2-interpolants
    #  yices2-const-arrays
    #  yices2-ite-test
      yices2-neg-numbers
      amalee-tests
)

foreach(test ${YICES2_TESTS})
  add_executable(${test}.out ${test}.cpp)
  target_include_directories (${test}.out PUBLIC "${PROJECT_SOURCE_DIR}/include")
  target_link_libraries(${test}.out smt-switch)
  target_link_libraries(${test}.out smt-switch-yices2)
  add_test(${test} ${test}.out)
endforeach()
