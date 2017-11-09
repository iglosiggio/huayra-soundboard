soundboard_sources = [
  'main.c',
  'soundboard-window.c',
  'category-widget.c',
  'sample-widget.c',
]

soundboard_deps = [
  dependency('gio-2.0', version: '>= 2.50'),
  dependency('gtk+-3.0', version: '>= 3.22'),
]

gnome = import('gnome')

soundboard_sources += gnome.compile_resources('soundboard-resources',
  'soundboard.gresource.xml',
  c_name: 'soundboard'
)

executable('soundboard', soundboard_sources,

  dependencies: soundboard_deps,
  install: true,
)
