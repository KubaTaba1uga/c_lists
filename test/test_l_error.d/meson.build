test_name = 'test_l_error.c'

test_src = files(test_name) + interfaces_h
test_objs = c_lists_lib.extract_objects(l_error)

test_l_error_exe = executable('test_l_error',
  sources: [
    test_src,
    cmock_gen_runner.process(test_name),
  ],
  objects: [
    test_objs
  ],
  include_directories: tests_include,
  dependencies: [unity_dependency, cmock_dependency],
    link_args: [
    # allow static variables testing
    '-zmuldefs',
  ]
)


test('test_l_error', test_l_error_exe)
