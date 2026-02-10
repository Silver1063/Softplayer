project(
  'softplayer',
  'cpp',
  version : '0.1',
  meson_version : '>= 1.3.0',
  default_options : ['warning_level=everything', 'cpp_std=c++14'],
)


sdl3_dep = dependency('sdl3', static: true)


dependencies = [
  sdl3_dep,
]

sources = [
  'main.cpp',
]

exe = executable(
  'softplayer',
  sources,
  install : true,
  dependencies : dependencies,
)

test('basic', exe)
