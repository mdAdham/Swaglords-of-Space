--Swag Dependencies

LibraryDir = {}

LibraryDir["SFML"] = "%{wks.location}/vendor/SFML/lib"

Library = {}

Library["sfml_main"] = "%{LibraryDir.SFML}/sfml-main.lib"

Library["sfml_window_s"] = "%{LibraryDir.SFML}/sfml-window-s.lib"
Library["sfml_graphics_s"] = "%{LibraryDir.SFML}/sfml-graphics-s.lib"
Library["sfml_system_s"] = "%{LibraryDir.SFML}/sfml-system-s.lib"
Library["sfml_audio_s"] = "%{LibraryDir.SFML}/sfml-audio-s.lib"

Library["sfml_window_s_d"] = "%{LibraryDir.SFML}/sfml-window-s-d.lib"
Library["sfml_graphics_s_d"] = "%{LibraryDir.SFML}/sfml-graphics-s-d.lib"
Library["sfml_system_s_d"] = "%{LibraryDir.SFML}/sfml-system-s-d.lib"
Library["sfml_audio_s_d"] = "%{LibraryDir.SFML}/sfml-audio-s-d.lib"

Library["opengl32"] = "opengl32.lib"
Library["openal32"] = "%{LibraryDir.SFML}/openal32.lib"
Library["winmm"] = "winmm.lib"
Library["gdi32"] = "gdi32.lib"
Library["flac"] = "%{LibraryDir.SFML}/flac.lib"
Library["vorbisenc"] = "%{LibraryDir.SFML}/vorbisenc.lib"
Library["vorbis"] = "%{LibraryDir.SFML}/vorbis.lib"
Library["vorbisfile"] = "%{LibraryDir.SFML}/vorbisfile.lib"
Library["ogg"] = "%{LibraryDir.SFML}/ogg.lib"
Library["ws2_32"] = "ws2_32.lib"
Library["freetype"] = "%{LibraryDir.SFML}/freetype.lib"