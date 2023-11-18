cmock_subproject = subproject('cmock')

unity_dependency = cmock_subproject.get_variable('unity_dep')
cmock_dependency = cmock_subproject.get_variable('cmock_dep')

tests_dependencies = [unity_dependency, cmock_dependency]

tests_include = [c_lists_include]

scripts = join_paths('..', 'scripts')

mock_script = find_program(scripts / 'create_mock.rb')

cmock_gen_runner = generator(
  find_program(scripts / 'create_runner.rb'),
  output: '@BASENAME@_Runner.c',
  arguments: ['@INPUT@', '@OUTPUT@'],
)

cmock_prog = find_program(scripts / 'create_mock.rb')

std_lib_mock = custom_target('std_lib_mock',
                             output: ['mock_cll_std_lib_interface.h', 'mock_cll_std_lib_interface.c'],
                             input: [std_lib_interface_h],
                             command: [cmock_prog, std_lib_interface_h, '@OUTDIR@'],
                           )

interfaces_h = files('interfaces.h')

subdir('test_ar_list.d')

