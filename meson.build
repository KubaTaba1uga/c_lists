# ******************************************************************************
# *    App
# ******************************************************************************
project('c_lists', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=true',
           'c_std=c99',
         ])

if get_option('enable_tests')
  add_project_arguments('-DENABLE_TESTS=1', language : 'c')
  add_project_arguments('-save-temps', language : 'c')    
endif


c_lists_include = [include_directories('include')]

_prefix_script = find_program('scripts' / 'generate_sources.py')

subdir('src')

# ******************************************************************************
# *    Array List
# ******************************************************************************
_arl_prefix = get_option('arl_prefix')
_arl_prefix_ = _arl_prefix + '_'

_arl_list_gen_sources = custom_target('arl_list_generated_sources',
                                      output: [_arl_prefix_ + 'list.c', _arl_prefix_ + 'list.h'],
                                      command: [_prefix_script, arl_list_file, _arl_prefix, '@OUTDIR@'])

add_global_arguments('-D' + _arl_prefix.to_upper() + '_VALUE_TYPE=' + get_option('arl_type'), language: 'c')

arl_lib = library(_arl_prefix,
                  include_directories: c_lists_include,                       
                  sources: [arl_list_sources + _arl_list_gen_sources],
                  # c_args: [
                  #   '-D' + _arl_prefix.to_upper() + '_VALUE_TYPE=' + get_option('arl_type'),
                  # ],
                  name_prefix: 'lib_')

arl_lib_dep = declare_dependency(sources: _arl_list_gen_sources[1],
                                 link_with: arl_lib,
                                 include_directories: arl_lib.private_dir_include())

# ******************************************************************************
# *    Tests
# ******************************************************************************
if get_option('enable_tests')
  subdir('test')
endif
