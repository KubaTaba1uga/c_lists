root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')

test_gardener_name = 'test_gardener.c'

test_gardener_src = files([src / 'gardener.h', src / 'gardener.c',  test_gardener_name])

test_gardener_exe = executable('test_gardener',
  sources: [
   test_gardener_src,
   cmock_gen_runner.process(test_gardener_name),
   cmock_gen_mock.process(utils / 'std_lib_utils.h')        
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ]
)

test('test_gardener', test_gardener_exe)
