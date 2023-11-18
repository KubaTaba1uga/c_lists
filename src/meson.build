c_lists_dir = [include_directories('.')]

ar_list_sources = files(
  'arl_list.c'
)

if get_option('enable_tests')
  ar_list_sources += files('cll_interfaces.h')
  subdir('interfaces')
endif

