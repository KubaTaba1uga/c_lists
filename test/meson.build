cmock_subproject = subproject('cmock')

unity_dependency = cmock_subproject.get_variable('unity_dep')
cmock_dependency = cmock_subproject.get_variable('cmock_dep')

tests_dependencies = [unity_dependency, cmock_dependency]

scripts = join_paths('..', 'scripts')

mock_script = find_program(scripts / 'create_mock.rb')

cmock_gen_runner = generator(
  find_program(scripts / 'create_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@'],
)

cmock_gen_mock = generator(
   find_program(scripts / 'create_mock.rb'),
   output: ['mock_@BASENAME@.c', 'mock_@BASENAME@.h', '@PLAINNAME@'],
   arguments: ['@INPUT@', '@BUILD_DIR@'],
)

interfaces_h = files('interfaces.h')

subdir('test_ar_list.d')
subdir('test_utils.d')
subdir('test_l_error.d')
