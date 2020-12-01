workspace "MVC"
	architecture "x64"
	location ("builds")
	
if _ACTION == "vs2017" then
   location ("builds/VisualStudio2017")
end
if _ACTION == "vs2019" then
   location ("builds/VisualStudio2019")
end
if _ACTION == "vs2015" then
   location ("builds/VisualStudio2015")
end
	
	configurations 
	{ 
		"Debug", 
        "Release",
    }
	filter "configurations:Debug"    defines { "DEBUG" }  symbols  "On"
    filter "configurations:Release"  defines { "NDEBUG" } optimize "On"
    
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


project  "MVC"
	if _ACTION == "vs2019" then
		cppdialect "C++17"
		location ("builds/VisualStudio2019/projects")
    end
	if _ACTION == "vs2017" then
		location ("builds/VisualStudio2017/projects")
    end
	if _ACTION == "vs2015" then
		location ("builds/VisualStudio2015/projects")
    end
	
	kind "ConsoleApp"

	local SOURCE_DIR = "source/*"
    files
    { 
      SOURCE_DIR .. "**.h", 
      SOURCE_DIR .. "**.hpp", 
      SOURCE_DIR .. "**.c",
      SOURCE_DIR .. "**.cpp",
    }
	
	local THIRD_PARTY_DIR = "thirdparty/"
	includedirs
	{
		SOURCE_DIR,
		
		THIRD_PARTY_DIR,
		THIRD_PARTY_DIR .. "glfw/include",
		THIRD_PARTY_DIR .. "nanogui/include",
		THIRD_PARTY_DIR .. "nanogui/ext/glad/include",
		THIRD_PARTY_DIR .. "nanogui/ext/nanovg/src",
		THIRD_PARTY_DIR .. "cs_signal/source",
	}
	
	
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		defines 
		{ 
			"_CRT_SECURE_NO_WARNINGS",
			"__WINDOWS_WASAPI__",
			"_USE_MATH_DEFINES", "NOMINMAX",
			"NANOGUI_USE_OPENGL", "NANOGUI_GLAD",
		}
		disablewarnings { "5030" , "4305", "4316", "4267"}
		vpaths 
		{
		  ["Header Files/*"] = { 
			SOURCE_DIR .. "**.h", 
			SOURCE_DIR .. "**.hpp",
		  },
		  ["Source Files/*"] = { 
			SOURCE_DIR .. "**.c", 
			SOURCE_DIR .. "**.cpp",
		  },
		}
		
		
	links 
	{ 
		"nanogui",
		"GLFW",
		"stb",
		"cs_signal",
	}
	libdirs { THIRD_PARTY_DIR .. "builds/bin/" .. outputdir .. "/**",
				 
		}
		


