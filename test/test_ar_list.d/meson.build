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
# TEST AR LIST LOGIC 2
################################################
test_file_name = 'test_ar_list_logic_2.c'
test_name = 'test_ar_list_logic_2'

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
