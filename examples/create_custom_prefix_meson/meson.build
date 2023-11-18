# ******************************************************************************
# *    App
# ******************************************************************************
project('example', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=true',
           'c_std=c99',
         ])

add_project_arguments('-save-temps', language : 'c')

###############
# DEPS
###############
c_lists_int = subproject('c_lists_int',
                                default_options: [
                                  'enable_tests=false',
                                  'arl_prefix=int',
                                  'arl_type=ababab'
                                ]
                               )

c_lists_char = subproject('c_lists_char',
                                default_options: [
                                  'enable_tests=false',
                                  'arl_prefix=chr',
                                  'arl_type=char'
                                ]
                               )

int_dep = c_lists_int.get_variable('arl_lib_dep')
char_dep = c_lists_char.get_variable('arl_lib_dep')

###############
# EXAMPLE
###############
main = executable('main', sources: ['main.c'], dependencies: [int_dep, char_dep])
