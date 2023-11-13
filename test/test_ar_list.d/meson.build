root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')
interfaces = join_paths(src, 'interfaces')

test_file_name = 'test_ar_list_logic.c'
test_name = 'test_ar_list_logic'

cmock_test_src = files([
  test_file_name, '..' / 'interfaces.h',
  src / 'ar_list.h', src / 'ar_list.c',
  src / 'l_error.h', src / 'l_error.c',
  utils / 'overflow_utils.h', utils / 'overflow_utils.c',
  utils / 'pointers_utils.h', utils / 'pointers_utils.c',
])

test_ar_list_exe = executable(test_name,
  sources: [
   cmock_test_src,
   cmock_gen_runner.process(test_file_name),
   cmock_gen_mock.process( interfaces / 'std_lib_interface.h')
  ],
  include_directories: [src, utils, interfaces],
  dependencies: [unity_dependency, cmock_dependency],
  link_args: [
    # allow static functions testing
    '-zmuldefs',
    
  ],
  c_args: ['-save-temps']
             
)

test(test_name, test_ar_list_exe)
