root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')
test_utils = join_paths('..', 'utils')

################################################
# TEST OVERFLOW UTILS
################################################
test_overflow_file = 'test_overflow_utils.c'
test_overflow_name = 'test_overflow_utils'

test_overflow_src = files([
  test_overflow_file,
  utils / 'overflow_utils.h', utils / 'overflow_utils.c',
])

test_overflow_exe = executable(test_overflow_name,
  sources: [
   test_overflow_src,
   cmock_gen_runner.process(test_overflow_file),
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
)

test(test_overflow_name, test_overflow_exe, suite: 'test_utils')

################################################
# TEST POINTERS UTILS
################################################
test_pointers_utils_file = 'test_pointers_utils.c'
test_pointers_utils_name = 'test_pointers_utils'

test_pointers_utils_src = files([
  test_pointers_utils_file,
  utils / 'pointers_utils.h', utils / 'pointers_utils.c',
])

test_pointers_utils_exe = executable(test_pointers_utils_name,
  sources: [
   test_pointers_utils_src,
   cmock_gen_runner.process(test_pointers_utils_file),
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
)

test(test_pointers_utils_name, test_pointers_utils_exe, suite: 'test_utils')
