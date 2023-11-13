c_lists_dir = include_directories('.')

sources = files(
  'ar_list.c', 'ar_list.h',
  'interfaces.h',
)

subdir('utils')

if get_option('enable_tests')
  subdir('interfaces')
endif

