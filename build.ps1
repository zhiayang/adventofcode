# meant to be executed from aoc/2018

$prog_year  = $args[0]
$prog_day   = $args[1]
$prog_lang  = $args[2]

$flax_compiler  = "D:\Projects\flax\build\meson-rel\flaxc.exe"
$flax_sysroot   = "D:\Projects\flax\build\sysroot"


if ([String]::IsNullOrWhiteSpace($prog_year)) {
	echo "year required"
	exit
}

if ([String]::IsNullOrWhiteSpace($prog_day)) {
	echo "day required"
	exit
}

if ([String]::IsNullOrWhiteSpace($prog_lang)) {
	$prog_lang = "cpp"
}


if ($prog_lang -ne "cpp" -and $prog_lang -ne "flax") {
    echo "language should be 'cpp' or 'flax'"
    exit
}

cls
echo "building AoC $prog_year day$prog_day ($prog_lang)"

if ($prog_lang -eq "cpp") {
    $compileFlags = @(
        "/nologo",
        "/GL",
        "/wd4624",
        "/wd4291",
        "/wd4141",
        "/wd4996",
        "/EHsc",
        "/std:c++17",
        "/Ilibs",
        "/Fe$prog_year\day$prog_day\prog.exe"
    )

    $optimisationFlags = @(
        "/O2",
        "/MD"
    )

    $linkerFlags = @(
        "/MACHINE:X64",
        "/NODEFAULTLIB:LIBCMTD.lib",
        "/NODEFAULTLIB:LIBCMT.lib"
    )

    cl.exe "$prog_year\day$prog_day\prog.cpp" @compileFlags $optimisationFlags /link $linkerFlags
    if ($LastExitCode -ne 0) {
        echo "compilation failed"
        exit
    }

    Remove-Item prog.obj

    echo "running $prog_year/day$prog_day`n"

    cd $prog_year\day$prog_day
        & ".\prog.exe"
    cd ..\..
}
else {
    # flax
    # note: we don't yet have EXE output, so use the jit.

    cd $prog_year\day$prog_day
        & $flax_compiler -sysroot $flax_sysroot -O3 -run "prog.flx"
    cd ..\..
}












# $projectDir     = "D:\Projects\flax"
# $objectDir      = "$projectDir\build\sysroot\windows\obj\x64\$BuildType"
# $outputDir      = "$projectDir\build\sysroot\windows\$BuildType"

# $llvmDir        = "D:\Projects\lib\llvm\$BuildType"
# $mpirDir        = "D:\Projects\lib\mpir\$BuildType"
# $mpfrDir        = "D:\Projects\lib\mpfr\$BuildType"

# $llvmLibDir     = "$llvmDir\lib"
# $llvmIncludeDir = "$llvmDir\include"

# $mpfrLibDir     = "$mpfrDir\lib"
# $mpfrIncludeDir = "$mpfrDir\include"

# $mpirLibDir     = "$mpirDir\lib"
# $mpirIncludeDir = "$mpirDir\include"


# if (Test-Path "$projectDir\source\unity.cpp") {
#     Remove-Item "$projectDir\source\unity.cpp"
# }


# $fileList = (Get-ChildItem "$projectDir\source" -File -Name -Recurse -Filter *.cpp)


# $includeList = @"
# #define NOMINMAX
# #define WIN32_LEAN_AND_MEAN 1
# #include <windows.h>

# #include "platform.h"

# #undef DO_NOT_INCLUDE_PRECOMPILE
# #include "precompile.h"

# #define DO_NOT_INCLUDE_PRECOMPILE

# "@

# Foreach ($file in $fileList) {
#     $includeList += "#include `"$file`"`n"
# }

# # ok, so what we want to do is to make a file.
# New-Item -Path source/ -Force -Name "unity.cpp" -Value $includeList | Out-Null



# # after we're done with that, we need to compile utf8rewind... sigh.

# New-Item -Path $objectDir\utf8rewind -ItemType Directory -Force | Out-Null
# New-Item -Path $outputDir -ItemType Directory -Force | Out-Null

# (Get-ChildItem "$projectDir\source" -File -Name -Recurse -Filter *.c) | ForEach-Object {
#     $basename = [io.path]::GetFileNameWithoutExtension($_)
#     cl.exe /nologo /c /EHsc source\$_ /Fo$objectDir\utf8rewind\$basename.obj
# }

# # ok, next link a library.
# $utf8Objs = (Get-ChildItem "$objectDir\utf8rewind" -File -Name -Filter *.obj) | ForEach-Object {
#     "$objectDir\utf8rewind\" + $_
# }

# lib.exe /nologo /out:$projectDir\build\sysroot\windows\obj\x64\$BuildType\utf8rewind.lib $utf8Objs


# $linkLibraries = @(
# 	"LLVMCore.lib",
# 	"LLVMSupport.lib",
# 	"LLVMTarget.lib",
# 	"LLVMPasses.lib",
# 	"LLVMAnalysis.lib",
# 	"LLVMGlobalISel.lib",
# 	"LLVMLibDriver.lib",
# 	"LLVMLinker.lib",
# 	"LLVMipo.lib",
# 	"LLVMBinaryFormat.lib",
# 	"LLVMMC.lib",
# 	"LLVMMCJIT.lib",
# 	"LLVMMCParser.lib",
# 	"LLVMMCDisassembler.lib",
# 	"LLVMObject.lib",
# 	"LLVMScalarOpts.lib",
# 	"LLVMVectorize.lib",
# 	"LLVMCodegen.lib",
# 	"LLVMTablegen.lib",
# 	"LLVMBitReader.lib",
# 	"LLVMBitWriter.lib",
# 	"LLVMInstrumentation.lib",
# 	"LLVMRuntimeDyld.lib",
# 	"LLVMInstCombine.lib",
# 	"LLVMInterpreter.lib",
# 	"LLVMExecutionEngine.lib",
# 	"LLVMSelectionDAG.lib",
# 	"LLVMTransformUtils.lib",
# 	"LLVMDebugInfoCodeView.lib",
# 	"LLVMDebugInfoDWARF.lib",
# 	"LLVMDebugInfoMSF.lib",
# 	"LLVMDebugInfoPDB.lib",
# 	"LLVMAsmPrinter.lib",
# 	"LLVMX86AsmPrinter.lib",
# 	"LLVMProfileData.lib",
# 	"LLVMX86AsmParser.lib",
# 	"LLVMX86Info.lib",
# 	"LLVMX86CodeGen.lib",
# 	"LLVMX86Utils.lib",
# 	"LLVMX86Desc.lib",
# 	"LLVMDlltoolDriver.lib",
# 	"mpfr.lib",
# 	"mpir.lib",
#     "$objectDir\utf8rewind.lib"
# )


# $compileFlags = @(
#     "/nologo",
#     "/GL",
#     "/wd4624",
#     "/wd4291",
#     "/wd4141",
#     "/wd4996",
#     "/EHsc",
#     "/std:c++17",
#     "/I$projectDir\source\include",
#     "/I$llvmIncludeDir",
#     "/I$mpirIncludeDir",
#     "/I$mpfrIncludeDir",
#     "/Fe$outputDir\flax-unity.exe",
#     "$projectDir\source\unity.cpp"
# )

# If ($BuildType -eq "Release")
# {
#     $optimisationFlags = @(
#         "/O2",
#         "/MD"
#     )
# }
# Else
# {
#     $optimisationFlags = @(
#         "/Od",
#         "/MDd",
#         "/Debug:fastlink"
#     )
# }

# $linkerFlags = @(
#     "/MACHINE:X64",
#     "/NODEFAULTLIB:LIBCMTD.lib",
#     "/NODEFAULTLIB:LIBCMT.lib",
#     "/LIBPATH:$llvmLibDir",
#     "/LIBPATH:$mpfrLibDir",
#     "/LIBPATH:$mpirLibDir"
# )

# cl.exe @compileFlags $optimisationFlags /link $linkerFlags $linkLibraries


# # copy the flax libraries.
# # robocopy $projectDir\libs $projectDir\build\sysroot\usr\local\lib\flaxlibs /e /nfl /ndl /njh /njs /nc /ns /np

# echo `n`n

# # ok, now run flax.

# $flaxArgs = @("-sysroot", "$projectDir\build\sysroot", "-run", "$projectDir\build\$TestProg.flx")
# & $outputDir\flax-unity.exe $flaxArgs