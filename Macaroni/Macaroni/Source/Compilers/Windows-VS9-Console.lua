require "Windows-VS9"

additionalCompilerArgs=additionalCompilerArgs .. " /MDd"

preprocessorDirectives["_CONSOLE"] = ""
