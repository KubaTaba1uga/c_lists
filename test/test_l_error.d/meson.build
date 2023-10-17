root = join_paths('..', '..')
src = join_paths(root, 'src')

test_name = 'test_l_error.c'

cmock_test_src = files([
  test_name,
  src / 'l_error.h', src / 'l_error.c',
])

test_l_error_exe = executable('test_l_error',
  sources: [
   cmock_test_src,
   cmock_gen_runner.process(test_name),
  ],
  include_directories: [src],
  dependencies: [unity_dependency, cmock_dependency],
    link_args: [
    # allow static variables testing
    '-zmuldefs',
  ]
)


test('test_l_error', test_l_error_exe)
