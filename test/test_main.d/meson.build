root = join_paths('..', '..')
src = join_paths(root, 'src')
utils = join_paths(src, 'utils')

test_main_name = 'test_main.c'

test_main_src = files([test_main_name, src / 'main.c', src / 'gardener.c', utils / 'std_lib_utils.c'])

test_main_exe = executable('test_main',
  sources: [
    test_main_src,
    cmock_gen_runner.process(test_main_name),
  ],
  include_directories: [src, utils],
  dependencies: [unity_dependency, cmock_dependency],
  c_args:['-DTEST'],
  link_args: [
    # allow static functions testing
    '-zmuldefs',
  ]
)

test('test_main', test_main_exe)
