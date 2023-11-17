################################################
# TEST OVERFLOW UTILS
################################################
test_overflow_utils_file = 'test_overflow_utils.c'
test_overflow_utils_name = 'test_overflow_utils'

test_overflow_utils_src = files(test_overflow_utils_file)
test_overflow_utils_objs = c_lists_lib.extract_objects(overflow_utils)

test_overflow_exe = executable(test_overflow_utils_name,
  sources: [
   test_overflow_utils_src,
   cmock_gen_runner.process(test_overflow_utils_file),
  ],
  objects: [
   test_overflow_utils_objs,    
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
)

test(test_overflow_utils_name, test_overflow_exe, suite: 'test_utils')

################################################
# TEST POINTERS UTILS
################################################
test_pointers_utils_file = 'test_pointers_utils.c'
test_pointers_utils_name = 'test_pointers_utils'

test_pointers_utils_src = files(test_pointers_utils_file)
test_pointers_utils_src += sources

test_pointers_utils_exe = executable(test_pointers_utils_name,
  sources: [
   test_pointers_utils_src,
   cmock_gen_runner.process(test_pointers_utils_file),
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  c_args: [
    '-DCLL_VALUE_TYPE=void *',
    '-DDEBUG_MOVE_POINTERS=true'
  ]

)

test(test_pointers_utils_name, test_pointers_utils_exe, suite: 'test_utils')
