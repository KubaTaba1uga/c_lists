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

###############
# DEPS
###############
c_lists_int = subproject('c_lists_int',
                                default_options: [
                                  'enable_tests=false',
                                  'arl_prefix=kuba',
                                  'arl_type=int'
                                ]
                               )

c_lists_char = subproject('c_lists_char',
                                default_options: [
                                  'enable_tests=false',
                                  'arl_prefix=dupa',
                                  'arl_type=char'
                                ]
                               )


