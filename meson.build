# ******************************************************************************
# *    App
# ******************************************************************************
project('c_lists', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=true',
           'c_std=c99',
         ],
       )

if get_option('enable_tests')
  add_project_arguments('-DENABLE_TESTS=1', language : 'c')
  add_project_arguments('-save-temps', language : 'c')    
endif


c_lists_include = [include_directories('include')]

_prefix_script = find_program('scripts' / 'generate_sources.py')

_prefix_gen = generator(_prefix_script,
                        output: ['@BASENAME@.c', '@BASENAME@.h'],
                        arguments: ['@INPUT@', '@EXTRA_ARGS@', '@BUILD_DIR@'],
                       )

subdir('src')

arl_list_sources += _prefix_gen.process(arl_list_file, extra_args: get_option('arl_prefix')) 

arl_list_lib = library('arl_list_lib', arl_list_sources,
                       include_directories: c_lists_include,
                       c_args: [
                         '-DCLL_VALUE_TYPE=' + get_option('arl_type'),
                       ]
                      )

arl_list_dep = declare_dependency(link_with: arl_list_lib, include_directories: c_lists_include)

# ******************************************************************************
# *    Tests
# ******************************************************************************
if get_option('enable_tests')
  subdir('test')
endif

# ******************************************************************************
# *    Examples
# ******************************************************************************
if get_option('enable_tests')
  subdir('examples')
endif
