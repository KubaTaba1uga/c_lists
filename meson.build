# ******************************************************************************
# *    App
# ******************************************************************************
project('c_lists', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=true',
           'c_std=c99'
         ]
       )

if get_option('enable_tests')
  add_project_arguments('-DENABLE_TESTS=1', language : 'c')
endif

subdir('src')

c_lists_lib = library('c_lists', sources, include_directories: c_lists_dir, c_args: '-save-temps')

c_lists_dep = declare_dependency(link_with: c_lists_lib, include_directories: c_lists_dir)
# ******************************************************************************
# *    Tests
# ******************************************************************************
if get_option('enable_tests')
  subdir('test')
endif
