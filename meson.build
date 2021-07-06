project('jpg-loaders-comparison', 'cpp')

cc = meson.get_compiler('cpp')
subloader_dep = [declare_dependency(
    include_directories : include_directories('.'),
    sources             : 'tvgLoader.h'
)]

subdir('jpg-turbo')
subdir('jpg-richgel')

executable(
	'main', 'main.cpp',
    dependencies: subloader_dep,
	include_directories : include_directories('.')
)
