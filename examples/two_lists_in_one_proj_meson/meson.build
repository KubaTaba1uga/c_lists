###############
# APP
###############
project('example', 'c',
         version: '0.0.1',
	 meson_version: '>=1.0.1',
	 default_options: [
           'werror=false',
           'c_std=c99',
         ])

###############
# DEPS
###############
chr_list_proj = subproject('chr_lists',
                         default_options: [
                           'enable_tests=false',
                           'arl_prefix=chr',
                           'arl_type=char',
                           ])
chr_list_dep = chr_list_proj.get_variable('arl_lib_dep')


int_list_proj = subproject('int_lists',
                         default_options: [
                           'enable_tests=false',
                           'arl_prefix=int',
                           'arl_type=int',
                           ])
int_list_dep = int_list_proj.get_variable('arl_lib_dep')

###############
# EXAMPLE
###############
main = executable('main', sources: ['main.c'], dependencies: [chr_list_dep, int_list_dep])

