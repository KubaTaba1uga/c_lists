root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')

test_name = 'test_ar_list_logic.c'


cmock_test_src = files([
  test_name,
  src / 'ar_list.h', src / 'ar_list.c',
  utils / 'std_lib_utils.h', utils / 'std_lib_utils.c'
])

test_ar_list_exe = executable('test_ar_list_logic',
  sources: [
   cmock_test_src,
   cmock_gen_runner.process(test_name)                   
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
)

test('test_ar_list_logic', test_ar_list_exe, priority: 999)
