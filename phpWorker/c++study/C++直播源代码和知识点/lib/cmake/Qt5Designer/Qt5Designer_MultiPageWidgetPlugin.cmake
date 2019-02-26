
add_library(Qt5::MultiPageWidgetPlugin MODULE IMPORTED)

_populate_Designer_plugin_properties(MultiPageWidgetPlugin RELEASE "designer/libcontainerextension.dylib")
_populate_Designer_plugin_properties(MultiPageWidgetPlugin DEBUG "designer/libcontainerextension.dylib")

list(APPEND Qt5Designer_PLUGINS Qt5::MultiPageWidgetPlugin)
