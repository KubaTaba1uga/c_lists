c_lists_dir = [include_directories('.')]

common_sources = files(
 'cll_error.c',
 'cll_interfaces.h',
)

ar_list_sources = files(
  'ar_list.c',
)

sources = common_sources
sources += ar_list_sources

if get_option('enable_tests')
  subdir('interfaces')
endif

