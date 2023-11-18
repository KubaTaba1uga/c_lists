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


c_lists_include = include_directories('include')

subdir('src')

_sources_gen = find_program('scripts' / 'generate_sources.py')

c_lists_gen = generator(_sources_gen,
                        output: ['@BASENAME@.c', '@BASENAME@.h'],
                        arguments: ['@INPUT', '@BUILD_DIR@'],
                      )

#c_lists_lib = library('c_lists', sources, include_directories: c_lists_include)

#c_lists_dep = declare_dependency(link_with: c_lists_lib, include_directories: c_lists_include)

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
