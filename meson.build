project(
  'Typr',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=3', 'cpp_std=c++14'],
)

raylib_inc = include_directories('include')
raylib_lib_dir = meson.current_source_dir() / 'lib'

raylib_dep = declare_dependency(
  include_directories: raylib_inc,
  link_args: ['-L' + raylib_lib_dir, '-lraylib']
)

src = [
  'src/app.cpp',
  'src/main.cpp',
  'src/renderer.cpp',
  'src/objects.cpp',
]

exe = executable(
  'typr',
  src,
  dependencies: [raylib_dep],
  install: true
)

test('basic', exe)
