# ******************************************************************************
# *    App
# ******************************************************************************
project('example', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=false',
           'c_std=c99',
         ])

add_project_arguments('-save-temps', language : 'c')

###############
# DEPS
###############
c_lists_proj = subproject('c_lists',
                         default_options: [
                           'enable_tests=false',
                           'arl_prefix=myprefix',
                           ])
c_lists_dep = c_lists_proj.get_variable('arl_lib_dep')

###############
# EXAMPLE
###############
main = executable('main', sources: ['main.c'], dependencies: [c_lists_dep])
