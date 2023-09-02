test_ar_list_name = 'test_ar_list.c'

cmock_test_src = files([
  test_ar_list_name
])

test_ar_list_exe = executable('test_ar_list',
  sources: [
   cmock_test_src,
   cmock_gen_runner.process(test_ar_list_name)                   
  ],
  dependencies: [unity_dependency],
)

test('test_ar_list_installation', test_ar_list_exe, priority: 999)
