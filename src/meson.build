c_lists_dir = [include_directories('.')]

common_sources = files(
 'cll_interfaces.h',
)

error_sources = files(
 'cll_error.c',  
)

common_sources += error_sources

if get_option('enable_tests')
  subdir('interfaces')
endif


ar_list_sources = files(
  'arl_list.c',
) + common_sources

sources = ar_list_sources


