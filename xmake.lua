add_rules("mode.debug", "mode.release")

target("test")
    set_kind("binary")
    set_languages("cxx17")
    add_includedirs("include")
    add_files("src/**.cpp")
    set_warnings("everything", "pedantic")
