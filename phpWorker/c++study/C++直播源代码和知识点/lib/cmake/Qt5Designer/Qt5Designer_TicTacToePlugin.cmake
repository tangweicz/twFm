
add_library(Qt5::TicTacToePlugin MODULE IMPORTED)

_populate_Designer_plugin_properties(TicTacToePlugin RELEASE "designer/libtaskmenuextension.dylib")
_populate_Designer_plugin_properties(TicTacToePlugin DEBUG "designer/libtaskmenuextension.dylib")

list(APPEND Qt5Designer_PLUGINS Qt5::TicTacToePlugin)
