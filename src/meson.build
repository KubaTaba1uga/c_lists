c_lists_dir = [include_directories('.')]

sources = files(
  'ar_list.c',
  'cll_interfaces.h',
)

subdir('utils')
subdir('common')

if get_option('enable_tests')
  subdir('interfaces')
endif

