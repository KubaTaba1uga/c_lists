root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')
test_utils = join_paths('..', 'utils')

test_name = 'test_ar_list_logic.c'


cmock_test_src = files([
  test_name,
  src / 'ar_list.h', src / 'ar_list.c',
  utils / 'overflow_utils.h', utils / 'overflow_utils.c',
  test_utils / 'test_utils.h', test_utils / 'test_utils.c',
])

test_ar_list_exe = executable('test_ar_list_logic',
  sources: [
   cmock_test_src,
   cmock_gen_runner.process('test_ar_list_logic.c'),
   cmock_gen_mock.process(utils / 'std_lib_utils.h')
  ],
  include_directories: [src, utils, test_utils],
  dependencies: [unity_dependency, cmock_dependency],
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ]
)

test('test_ar_list_logic', test_ar_list_exe)
