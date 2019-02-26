
add_library(Qt5::WorldTimeClockPlugin MODULE IMPORTED)

_populate_Designer_plugin_properties(WorldTimeClockPlugin RELEASE "designer/libworldtimeclockplugin.dylib")
_populate_Designer_plugin_properties(WorldTimeClockPlugin DEBUG "designer/libworldtimeclockplugin.dylib")

list(APPEND Qt5Designer_PLUGINS Qt5::WorldTimeClockPlugin)
