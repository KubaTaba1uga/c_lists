ar_list_test_sources = arl_list_sources + interfaces_h
ar_list_test_linker_flags = [
  '-zmuldefs',
]

################################################
# TEST AR LIST LOGIC
################################################
test_file_name = 'test_ar_list_logic.c'
test_name = 'test_ar_list_logic'

test_src = files(test_file_name)
test_src += ar_list_test_sources

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,
)

test(test_name, test_ar_list_exe)

################################################
# TEST AR LIST LOGIC (void *)
################################################
test_file_name = 'test_ar_list_void_ptr.c'
test_name = 'test_ar_list_logic_void_ptr'

test_src = files(test_file_name)
test_src += ar_list_test_sources

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,
  c_args: [
    '-DCLL_VALUE_TYPE=void *',
  ]
)

test(test_name, test_ar_list_exe)

################################################
# TEST AR LIST LOGIC (char)
################################################
test_file_name = 'test_ar_list_char.c'
test_name = 'test_ar_list_logic_char'

test_src = files(test_file_name)
test_src += ar_list_test_sources

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,
  c_args: [
    '-DCLL_VALUE_TYPE=char',
  ]
)

test(test_name, test_ar_list_exe)

################################################
# TEST OVERFLOW UTILS
################################################
test_overflow_utils_file = 'test_overflow_utils.c'
test_overflow_utils_name = 'test_overflow_utils'

test_overflow_utils_src = files(test_overflow_utils_file)
test_overflow_utils_src += ar_list_test_sources

test_overflow_exe = executable(test_overflow_utils_name,
  sources: [
   test_overflow_utils_src,
   cmock_gen_runner.process(test_overflow_utils_file),
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,
)

test(test_overflow_utils_name, test_overflow_exe, suite: 'test_utils')


################################################
# TEST POINTERS UTILS
################################################
test_pointers_utils_file = 'test_pointers_utils.c'
test_pointers_utils_name = 'test_pointers_utils'

test_pointers_utils_src = files(test_pointers_utils_file)
test_pointers_utils_src += ar_list_test_sources


test_pointers_utils_exe = executable(test_pointers_utils_name,
  sources: [
   test_pointers_utils_src,
   cmock_gen_runner.process(test_pointers_utils_file),
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,  
  c_args: [
    '-DCLL_VALUE_TYPE=void *',
    '-DDEBUG_MOVE_POINTERS=true'
  ]

)

test(test_pointers_utils_name, test_pointers_utils_exe, suite: 'test_utils')

################################################
# TEST ERRORS UTILS
################################################
test_name = 'test_errors_utils.c'

test_src = files(test_name)
test_src += ar_list_test_sources

test_l_error_exe = executable('test_errors_utils',
  sources: [
    test_src,
    cmock_gen_runner.process(test_name),
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: ar_list_test_linker_flags,
)


test('test_l_error', test_l_error_exe)
