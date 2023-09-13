root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')
test_utils = join_paths('..', 'utils')

test_overflow_name = 'test_overflow_utils.c'

test_overflow_src = files([
  test_overflow_name,
  utils / 'overflow_utils.h', utils / 'overflow_utils.c',
])

test_overflow_exe = executable('test_overflow_utils',
  sources: [
   test_overflow_src,
   cmock_gen_runner.process(test_overflow_name),
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
)

test('test_utils', test_overflow_exe)
