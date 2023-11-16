################################################
# TEST AR LIST LOGIC
################################################
test_file_name = 'test_ar_list_logic.c'
test_name = 'test_ar_list_logic'

test_src = files(test_file_name) + interfaces_h
test_objs = c_lists_lib.extract_all_objects(recursive: true)

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  objects: [
    test_objs
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ],         
)

test(test_name, test_ar_list_exe)

################################################
# TEST AR LIST LOGIC (void *)
################################################
test_file_name = 'test_ar_list_void_ptr.c'
test_name = 'test_ar_list_logic_void_ptr'

test_src = files(test_file_name) + interfaces_h
test_src += sources

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ],
  c_args: [
    '-DCLL_VALUE_TYPE=void *'
  ]
)

test(test_name, test_ar_list_exe)

################################################
# TEST AR LIST LOGIC (char)
################################################
test_file_name = 'test_ar_list_char.c'
test_name = 'test_ar_list_logic_char'

test_src = files(test_file_name) + interfaces_h
test_src += sources

test_ar_list_exe = executable(test_name,
  sources: [
   test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process(std_lib_interface_h)
  ],
  include_directories: tests_include,
  dependencies: tests_dependencies,
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ],
  c_args: [
    '-DCLL_VALUE_TYPE=char',
    '-Werror'
  ]
)

test(test_name, test_ar_list_exe)
